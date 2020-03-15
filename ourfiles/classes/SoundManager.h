#include <tutorial\tutorial\sandBox\include\irrKlang.h>
class SoundManager
{
public:
	SoundManager();
	~SoundManager();
	void backgroundSound();
	void stopbackgroundSound();
	void setVolume(int v);
	void attack();
	void miss();
	void levelup();
	void special();
	void Ending();
	bool isOn = false;

private:
	irrklang::ISoundEngine* soundeffects;
	irrklang::ISound* voice;
	
};

