#pragma once

#include "Module.h"
#include <queue>
#include "SDL2/SDL_audio.h"
#include "Vector2D.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_thread.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libswresample/swresample.h>
}

struct SDL_Texture;

class CutscenePlayer : public Module
{
public:

	CutscenePlayer();

	virtual ~CutscenePlayer();

	bool Awake(pugi::xml_node& conf);

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	bool OpenCodecContext(int* index);
	bool OpenVideoCodecContext(int index);
	bool OpenAudioCodecContext(int index);
	bool ConvertPixels(int videoIndex, int audioIndex);
	bool AllocImage(AVFrame* dstFrame);
	void RenderCutscene();
	//void SelectCharacter();
	void ProcessAudio();
	void audioCallback(void* userdata, Uint8* stream, int len);
	void audioCallbackProcess(Uint8* stream, int len);

public:
	int streamIndex = -1;
	AVFormatContext* formatContext;
	AVCodecContext* videoCodecContext;
	AVCodecContext* audioCodecContext;

	SDL_AudioDeviceID audioDevice;
	int audioStreamIndex;

	SDL_Texture* renderTexture;
	//SDL_Texture* texture1;
	//SDL_Texture* texture2;
	SDL_Rect renderRect;
	bool running;

	SDL_Rect rect1 = { 300,100,250,500 };
	SDL_Rect rect2 = { 600,100,250,500 };
	Vector2D position1 ;
	Vector2D position2 ;
	bool isHover1 = false;
	bool isHover2 = false;

	std::queue<AVPacket> audioBuffer;
};