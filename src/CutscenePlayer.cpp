#include "CutscenePlayer.h"
#include "Engine.h"
#include "Textures.h"
#include "Log.h"
#include "Render.h"
#include "Window.h"
#include "Audio.h"
#include "Scene.h"

CutscenePlayer::CutscenePlayer() : Module()
{
    name = "cutsceneplayer";
}

CutscenePlayer::~CutscenePlayer()
{}

bool CutscenePlayer::Awake(pugi::xml_node& config)
{
    LOG("Loading CutscenePlayer");
    bool ret = true;

    return ret;
}

bool CutscenePlayer::Start()
{
    position1 = Vector2D(300, 500);
    position2 = Vector2D(600, 500);
    // File path of the video to be played
    const char* file = "Assets/Video/video.mp4";
    //const char* file = "Assets/Video/sample-5s.mp4";

    // Allocate memory for the format context
    formatContext = avformat_alloc_context();

    // Open the input video file with FFMPEG
    if (avformat_open_input(&formatContext, file, NULL, NULL) < 0) {
        LOG("Failed to open input file");
        avformat_close_input(&formatContext);
        avformat_free_context(formatContext);
        return true;
    }

    // Find input stream information
    if (avformat_find_stream_info(formatContext, NULL) < 0) {
        LOG("Failed to find input stream information");
        avformat_close_input(&formatContext);
        avformat_free_context(formatContext);
        return true;
    }

    // Dump format information
    av_dump_format(formatContext, 0, file, 0);

    // Open codec context for video and audio streams
    OpenCodecContext(&streamIndex);

    // Create SDL texture for rendering video frames
    renderTexture = SDL_CreateTexture(Engine::GetInstance().render->renderer, SDL_PIXELFORMAT_YV12,
        SDL_TEXTUREACCESS_STREAMING, videoCodecContext->width, videoCodecContext->height);
    if (!renderTexture) {
        LOG("Failed to create texture - %s\n", SDL_GetError());
        return true;
    }

    // Set up SDL rectangle for video rendering
	renderRect = { 0, 0, videoCodecContext->width, videoCodecContext->height };

    // Load additional textures for rendering
    //texture1 = app->tex->Load("Assets/rakan.png");
    //texture2 = app->tex->Load("Assets/xayah.png");

    // Set the module state to running
    running = true;
    return true;
}

bool CutscenePlayer::OpenCodecContext(int* index)
{
    // Find video stream in the file
    int videoIndex = av_find_best_stream(formatContext, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
    if (videoIndex < 0) {
        LOG("Failed to find video stream in input file\n");
        return true;
    }

    // Find audio stream in the file
    int audioIndex = av_find_best_stream(formatContext, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
    if (audioIndex < 0) {
        LOG("Failed to find audio stream in input file\n");
        // No audio stream found, continue without audio
    }

    // Open video codec context
    if (OpenVideoCodecContext(videoIndex)) {
        return true;
    }

    // Open audio codec context if audio stream found
    if (audioIndex >= 0 && OpenAudioCodecContext(audioIndex)) {
        return true;
    }

    *index = videoIndex;

    return false;
}

bool CutscenePlayer::OpenVideoCodecContext(int videoIndex)
{
    // Find the decoder for the video codec
    const AVCodec* codec = avcodec_find_decoder(formatContext->streams[videoIndex]->codecpar->codec_id);
    if (!codec) {
        LOG("Failed to find video codec!\n");
        return true;
    }

    // Allocate a codec context for the decoder
    videoCodecContext = avcodec_alloc_context3(codec);
    if (!videoCodecContext) {
        LOG("Failed to allocate the video codec context\n");
        return true;
    }

    // Copy video codec parameters to the decoder context
    if (avcodec_parameters_to_context(videoCodecContext, formatContext->streams[videoIndex]->codecpar) < 0) {
        LOG("Failed to copy video codec parameters to decoder context!\n");
        return true;
    }

    // Open the video codec
    if (avcodec_open2(videoCodecContext, codec, NULL) < 0) {
        LOG("Failed to open video codec\n");
        return true;
    }

    return false;
}

Uint32 audioTimerCallback(Uint32 interval, void* param)
{
    CutscenePlayer* cutscenePlayer = static_cast<CutscenePlayer*>(param);
    if (cutscenePlayer) {
        cutscenePlayer->ProcessAudio();
    }
    return interval;
}

bool CutscenePlayer::OpenAudioCodecContext(int audioIndex)
{
	// Find the decoder for the audio codec
    const AVCodec* codec = avcodec_find_decoder(formatContext->streams[audioIndex]->codecpar->codec_id);
    if (!codec) {
        LOG("Failed to find audio codec!\n");
        return true;
    }

	// Allocate a codec context for the decoder
    audioCodecContext = avcodec_alloc_context3(codec);
    if (!audioCodecContext) {
        LOG("Failed to allocate the audio codec context\n");
        return true;
    }

	// Copy audio codec parameters to the decoder context
    if (avcodec_parameters_to_context(audioCodecContext, formatContext->streams[audioIndex]->codecpar) < 0) {
        LOG("Failed to copy audio codec parameters to decoder context!\n");
        return true;
    }

	// Open the audio codec
    if (avcodec_open2(audioCodecContext, codec, NULL) < 0) {
        LOG("Failed to open audio codec\n");
        return true;
    }

	// Set up SDL audio device
    SDL_AudioSpec wantedSpec, obtainedSpec;
    wantedSpec.freq = audioCodecContext->sample_rate;
    switch (audioCodecContext->sample_fmt) {
    case AV_SAMPLE_FMT_U8:
    case AV_SAMPLE_FMT_U8P:
        wantedSpec.format = AUDIO_U8;
        break;
    case AV_SAMPLE_FMT_S16:
    case AV_SAMPLE_FMT_S16P:
        wantedSpec.format = AUDIO_S16SYS;
        break;
    case AV_SAMPLE_FMT_S32:
    case AV_SAMPLE_FMT_S32P:
        wantedSpec.format = AUDIO_S32SYS;
        break;
    case AV_SAMPLE_FMT_FLT:
    case AV_SAMPLE_FMT_FLTP:
        wantedSpec.format = AUDIO_F32SYS;
        break;
    }
    wantedSpec.channels = 2;
    wantedSpec.silence = 0;
    wantedSpec.samples = audioCodecContext->frame_size;
    wantedSpec.callback = NULL;  // Set the callback here
    wantedSpec.userdata = this;  // Pass the instance of the class to the callback

	// Open the audio device
    audioDevice = SDL_OpenAudioDevice(NULL, 0, &wantedSpec, &obtainedSpec, 0);
    if (audioDevice == 0) {
        LOG("Failed to open audio device %s\n", SDL_GetError());
        return true;
    }

    SDL_AddTimer((1000 * wantedSpec.samples) / wantedSpec.freq, audioTimerCallback, this);

	// Start audio playback
    SDL_PauseAudioDevice(audioDevice, 0);

    system("cls");
    return false;
}

void CutscenePlayer::ProcessAudio()
{
    // Allocate memory for an audio frame
    AVFrame* frame = av_frame_alloc();
    if (!frame) {
        return;
    }

    Uint8* stream;
    AVPacket packet;

    // Process each packet in the audio buffer
    if (!audioBuffer.empty()) {
        // Get the first packet from the audio buffer
        packet = audioBuffer.front();
        audioBuffer.pop();

        // Send the packet to the audio decoder
        int ret = avcodec_send_packet(audioCodecContext, &packet);
        if (ret < 0) {
            av_frame_free(&frame);
            return;
        }

        // Receive decoded audio frame
        ret = avcodec_receive_frame(audioCodecContext, frame);
        if (ret < 0) {
            av_frame_free(&frame);
            return;
        }

        // Get the pointer to the audio data
        stream = reinterpret_cast<Uint8*>(frame->data[0]);

        // Queue the audio data for playback
        SDL_QueueAudio(audioDevice, stream, frame->linesize[0]);

        // Unreference the frame
        av_frame_unref(frame);
    }
    
    // Free memory allocated for the audio frame
    av_frame_free(&frame); 
}

bool CutscenePlayer::ConvertPixels(int videoIndex, int audioIndex)
{
    // Calculate time per frame based on the average frame rate of the video stream
    int time = 1000 * formatContext->streams[videoIndex]->avg_frame_rate.den
                    / formatContext->streams[videoIndex]->avg_frame_rate.num;
   
    AVPacket packet; 
    
    // Allocate memory for source and destination frames
    AVFrame* srcFrame = av_frame_alloc();
    if (!srcFrame) {
        return false;
    }
    AVFrame* dstFrame = av_frame_alloc();
    if (!dstFrame) {
        av_frame_free(&srcFrame);
        return false;
    }

    // Allocate memory for the image data of the destination frame
    AllocImage(dstFrame);

    // Create a scaling context to convert the pixel format of the video frames
    struct SwsContext* sws_ctx = sws_getContext(
        videoCodecContext->width, videoCodecContext->height, videoCodecContext->pix_fmt,
        videoCodecContext->width, videoCodecContext->height, AV_PIX_FMT_YUV420P,
        SWS_BILINEAR, NULL, NULL, NULL);

    // Loop through each packet in the video stream
    while (av_read_frame(formatContext, &packet) >= 0 && running) 
    {
        if (packet.stream_index == videoIndex) 
        {
            // Send packet to video decoder
            avcodec_send_packet(videoCodecContext, &packet);
            // Receive decoded frame
            int ret = avcodec_receive_frame(videoCodecContext, srcFrame);
            if (ret) continue;

            // Scale the source frame to the destination frame
            sws_scale(sws_ctx, (uint8_t const* const*)srcFrame->data,
                srcFrame->linesize, 0, videoCodecContext->height,
                dstFrame->data, dstFrame->linesize);

            // Update the SDL texture with the scaled YUV data
            SDL_UpdateYUVTexture(renderTexture, &renderRect,
                dstFrame->data[0], dstFrame->linesize[0],
                dstFrame->data[1], dstFrame->linesize[1],
                dstFrame->data[2], dstFrame->linesize[2]
            );

            // Render the video
            RenderCutscene();
			//SelectCharacter();

            // Wait for the specified time before processing the next frame
            SDL_Delay(time);

            // Unreference the packet to release its resources
            av_packet_unref(&packet);
        }
        else if (packet.stream_index == audioIndex) 
        {
            // Push to the audio buffer
            audioBuffer.push(packet);
        }
    }

    // Free memory allocated for the destination frame
    av_freep(&dstFrame->data[0]);

    // Free memory allocated for source and destination frames
    av_frame_free(&srcFrame);
    av_frame_free(&dstFrame);

    return false;
}

bool CutscenePlayer::AllocImage(AVFrame* image)
{
    // Set the pixel format, width, and height of the image frame
    image->format = AV_PIX_FMT_YUV420P;
    image->width = videoCodecContext->width;
    image->height = videoCodecContext->height;

    // Allocate memory for the image frame
    av_image_alloc(image->data, image->linesize,
        image->width, image->height, (AVPixelFormat)image->format, 32);

    return false;
}

void CutscenePlayer::RenderCutscene()
{
    SDL_RenderClear(Engine::GetInstance().render->renderer);

    SDL_RenderCopy(Engine::GetInstance().render->renderer, renderTexture, NULL, NULL);

    //if (isHover1) app->render->DrawTexture(texture1, position1.x, position1.y, NULL);
    //if (isHover2) app->render->DrawTexture(texture2, position2.x, position2.y, NULL);

    SDL_RenderPresent(Engine::GetInstance().render->renderer);
}
/*
void CutscenePlayer::SelectCharacter()
{
    SDL_Event event;
    SDL_PollEvent(&event);
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
    {
		running = false;
		LOG("Mouse clicked");
        app->fade->Fade(this, (Module*)app->scene, 10.0f);
        app->sceneMenu->Disable();
        app->map->Enable();
        app->entityManager->Enable();
	}

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    if (mouseX >= rect1.x && mouseX <= rect1.x + rect1.w &&
        mouseY >= rect1.y && mouseY <= rect1.y + rect1.h) {
        isHover1 = true;
    }
    else {
        isHover1 = false;
    }

    if (mouseX >= rect2.x && mouseX <= rect2.x + rect2.w &&
        mouseY >= rect2.y && mouseY <= rect2.y + rect2.h) {
        isHover2 = true;
    }
    else {
        isHover2 = false;
    }
	
}
*/
bool CutscenePlayer::Update(float dt)
{

    return true;
}

bool CutscenePlayer::CleanUp()
{
    LOG("Freeing cutscene player");

	// Close the vide and audio codec contexts
    avcodec_close(videoCodecContext);
    avcodec_close(audioCodecContext);

	// Close and free the format contexts
    avformat_close_input(&formatContext);
	avformat_free_context(formatContext);

	// Close audio device
    SDL_CloseAudioDevice(audioDevice);

    // Destroy textures
	SDL_DestroyTexture(renderTexture);
	//SDL_DestroyTexture(texture1);
	//SDL_DestroyTexture(texture2);

    return true;
}

void CutscenePlayer::audioCallback(void* userdata, Uint8* stream, int len)
{
    CutscenePlayer* cutscenePlayer = static_cast<CutscenePlayer*>(userdata);
    cutscenePlayer->audioCallbackProcess(stream, len);
}

void CutscenePlayer::audioCallbackProcess(Uint8* stream, int len)
{
    int bufferSize = len / 2; // For 16-bit stereo, this works well.
    int bytesPerSample = 2; // 16-bit samples

    // Process audio frames and copy to the SDL audio buffer
    while (len > 0 && !audioBuffer.empty()) {
        AVPacket packet = audioBuffer.front();
        audioBuffer.pop();

        int ret = avcodec_send_packet(audioCodecContext, &packet);
        if (ret < 0) {
            av_packet_unref(&packet);
            continue;
        }

        AVFrame* audioFrame = av_frame_alloc();
        ret = avcodec_receive_frame(audioCodecContext, audioFrame);
        if (ret < 0) {
            av_frame_free(&audioFrame);
            continue;
        }

        int frameSize = audioFrame->linesize[0] / bytesPerSample;
        if (frameSize > len) frameSize = len;

        memcpy(stream, audioFrame->data[0], frameSize);
        len -= frameSize;
        stream += frameSize;
        av_frame_free(&audioFrame);
    }
}


