#ifndef _H264LIVEFRAMEDSOURCE_HH
#define _H264LIVEFRAMEDSOURCE_HH

#include <ByteStreamFileSource.hh>

class H264LiveFramedSource : public ByteStreamFileSource
{
public:
	static H264LiveFramedSource* createNew(UsageEnvironment& env, unsigned preferredFrameSize = 0, unsigned playTimePerFrame = 0);

protected:
	H264LiveFramedSource(UsageEnvironment& env, unsigned preferredFrameSize, unsigned playTimePerFrame);
	~H264LiveFramedSource();

private:
	//重定义虚函数
	virtual void doGetNextFrame();

/*	virtual unsigned int maxFrameSize() const
	{
		return 150000 - 10;
	}
*/
	FILE *inpf;
	unsigned char *inBuf;
	unsigned char *curP;
	int inLen;
	int nFrameRate;
};

#endif