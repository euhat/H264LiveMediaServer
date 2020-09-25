#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"
#include "h264LiveVideoServerMediaSubssion.hh"

UsageEnvironment* env;

// True���������Ŀͻ������Ǵӵ�ǰ��һ���ͻ����Ѿ����ŵ���λ�ÿ�ʼ����
// False��ÿ���ͻ��˶���ͷ��ʼ����Ӱ��Ƶ�ļ�
Boolean reuseFirstSource = True;

//�ú�����ӡ�����Ϣ
static void announceStream(RTSPServer* rtspServer, ServerMediaSession* sms, char const* streamName); 

int main(int argc, char** argv) 
{
	//�����������������ʼ��ʹ�û���
	TaskScheduler* scheduler = BasicTaskScheduler::createNew();
	env = BasicUsageEnvironment::createNew(*scheduler);
	UserAuthenticationDatabase* authDB = NULL;

	//����RTSP����������ʼ����ģ�ͻ��˵�����
	//ע������Ķ˿ںŲ���Ĭ�ϵ�554�˿ڣ���˷���URLʱ����ָ���ö˿ں�
	RTSPServer* rtspServer = RTSPServer::createNew(*env, 8554, authDB);
	if (rtspServer == NULL) 
	{
		*env << "Failed to create RTSP server: " << env->getResultMsg() << "\n";
		exit(1);
	}

	char const* descriptionString = "Session streamed by \"h264LiveMediaServer\"";

	//�����֣�ý����
	char const* streamName = "h264";

	//���ͻ��㲥ʱ��Ҫ����������streamName������RTSP�������㲥�����ĸ�����  
	//����ý��Ự�������ֺ��ļ����Ķ�Ӧ��ϵ��ͨ�������ӻỰ���������ġ�ý��Ự�ԻỰ�������Ự����ʱ�䡢�����ֵ���Ự�йص���Ϣ���й���  
	//��2������:ý������3:ý����Ϣ��4:ý������  
	ServerMediaSession* sms= ServerMediaSession::createNew(*env, streamName, streamName ,descriptionString);

	//�޸�Ϊ�Լ�ʵ�ֵ�H264LiveVideoServerMediaSubssion
	sms->addSubsession(H264LiveVideoServerMediaSubssion::createNew(*env, reuseFirstSource));

	//Ϊrtspserver���session 
	rtspServer->addServerMediaSession(sms);

	//��Ӧ��Ϣ����׼���
	announceStream(rtspServer, sms, streamName);
    
	//�����¼�ѭ�������׽��ֵĶ�ȡ�¼��Ͷ�ý���ļ�����ʱ���Ͳ����������ѭ������ɡ� 
	env->taskScheduler().doEventLoop();

	return 0; 
}

static void announceStream(RTSPServer* rtspServer, ServerMediaSession* sms,char const* streamName) 
{
	char* url = rtspServer->rtspURL(sms);
	UsageEnvironment& env = rtspServer->envir();
	env << "\n\"" << streamName << "\" stream\"\n";
	env << "Play this stream using the URL \"" << url << "\"\n";
	delete[] url;
}
