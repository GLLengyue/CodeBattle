/*�Ի��򳡾���ľ���ʵ��*/
#include "DialogLayer.h"
//#include "QuizLayer.h"
#include <string>
#include <cctype>
#include <stdio.h>


using namespace std;
size_t index = 0;
size_t pos;
size_t index1 = 0;
size_t pos1;
size_t index2 = 0;
size_t pos2;
string s1,s2,ss,ss2;
string object;
string npcName[10];
int quizNumber = 2;
bool endJudge, quizJudge = false;

DialogLayer *DialogLayer::create(string objectName)
{
	auto aLayer = new DialogLayer;
	object = objectName;
	aLayer->init();
	return aLayer;
}

bool DialogLayer::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!CCLayer::init());
		this->setTouchEnabled(true);
		bRet = true;
	} while (0);

	//���̼���
	this->schedule(schedule_selector(DialogLayer::update), 0.1f);
	auto listener = EventListenerKeyboard::create();

	setNumber(1);

	using std::string;
	char *c = ".txt";
	char filename[10];
	//��int i��char *c����
	sprintf(filename, "%d%s", dialogNumber, c);
	puts(filename);
	log(filename);

	//����ļ���ϵͳ�ľ���·��
	string filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filename);
	log(filepath.c_str());
	//��ȡ���ֽ�������ȡʧ����Ϊ0
	ssize_t len = 0;
	//��ȡ������  
	unsigned char *data = CCFileUtils::sharedFileUtils()->getFileData(filepath.c_str(), "rb", &len);
	string str = (char*)data;

	//��ӷָ����Ų���ȡ�䳤��
	const char* delim = ";;";
	const int l = strlen(delim);
	const char* delim1 = "//";
	const int l1 = strlen(delim1);

	//��öԻ�������ߵ��ߵ�����s1,s2
	pos1 = str.find(delim1, index1);	
	index1 = pos1 + l1;
	//��str�е�index1λ�ÿ�ʼѰ��delim1
	pos1 = str.find(delim1, index1);
	//��ȡ��index1λ�ÿ�ʼ������Ϊpos1 - index1���ַ���
	s1 = str.substr(index1, pos1 - index1);

	index1 = pos1 + l1;
	pos1 = str.find(delim1, index1);
	s2 = str.substr(index1, pos1 - index1);
	log(s1.c_str());
	log(s2.c_str());


	npcName[0] = s1;
	npcName[1] = s2;
	for (int j = 2; j < 10; j++) {
		if (npcName[j-1].compare("end") != 0) {
			index1 = pos1 + l;
			pos1 = str.find(delim1, index1);
			npcName[j] = str.substr(index1, pos1 - index1);
		}
		else {
			break;
		}
	}

	//�����жϸ�npc�Ƿ񴥷�Quiz����
	for (int j = 0; j < 10; j++) {
		if (npcName[j] == object) {
			quizJudge = true;
		}
	}

	//����Quiz����
	if ((dialogNumber == quizNumber) && (quizJudge)){
		log("Quiz");
		//���Quiz�������һ�������ı�
		if (false /*�⿪����*/) {
			dialogNumber = dialogNumber + 1;
		}
		//δ���Quiz����ʾ��Ӧ�Ĺ̶��Ի�
		else {
			for (int j = 0; j < 10; j++) {
				if (npcName[j] == object) {
					log("match");
					//���öԻ��򱳾�
					setDialogBackground("dialog2.png");

					listener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {
						log("key pressed");
						keys[keyCode] = true;

						if (keyCode == EventKeyboard::KeyCode::KEY_SPACE) {
							if (!endJudge) {
								//ֱ���ҵ���name���ڵ�λ��
								pos = str.find(object, 0);
								index = pos + object.length();
								pos = str.find(object, index);
								index = pos + object.length();

								pos = str.find(delim, index);
								index = pos + l;
								pos = str.find(delim, index);
								ss = str.substr(index, pos - index);
								setDialogist(ss);

								//��ȡ����ʾ�Ի�����
								index = pos + l;
								pos = str.find(delim, index);
								ss = str.substr(index, pos - index);
								DialogLayer::i = 0;
								this->schedule(schedule_selector(DialogLayer::wordUpdate), 0.02f);

								endJudge = true;
							}
							else {
								this->removeChildByTag(2);
								auto nowScene = Director::getInstance()->getRunningScene();
								nowScene->removeChildByTag(2);
							}

						}
					};

				}
			}
		}
		log("end Quiz");
	}

	else {
		log("main dialog");
		//�������߶Ի�
		log(object.c_str());
		log(s2.c_str());
		//���öԻ��򱳾�
		setDialogBackground("dialog2.png");
		if (s2.compare(object)==0) {
			log("main dialog1");
			listener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {
				log("key pressed");
				keys[keyCode] = true;
				
				pos = str.find(delim, index);
				index = pos + l;
				pos = str.find(delim, index);
				ss = str.substr(index, pos - index);

				if (keyCode == EventKeyboard::KeyCode::KEY_SPACE) {
					if ((ss.compare("dialog")) == 0) {
						index = pos + l;
						pos = str.find(delim, index);
						ss = str.substr(index, pos - index);
						log(ss.c_str());

						//��������
						auto portrait = Sprite::create(ss + ".png");
						portrait->setTag(3);
						portrait->setAnchorPoint(Vec2(0.5, 0.5));
						//ƥ������
						if (ss == s1) {
							portrait->setPosition(ccp(winSize.width / 3, winSize.height*0.618));
						}
						else {
							portrait->setPosition(ccp(winSize.width * 5 / 3, winSize.height*0.618));
							//��ת����
							portrait->setScaleX(-1.0f);
						}

						//��ȡ����ʾ�Ի���
						index = pos + l;
						pos = str.find(delim, index);
						ss = str.substr(index, pos - index);
						setDialogist(ss);


						this->removeChildByTag(3);
						this->addChild(portrait, -1);

						//��ȡ����ʾ�Ի�����
						index = pos + l;
						pos = str.find(delim, index);
						ss = str.substr(index, pos - index);
						DialogLayer::i = 0;
						this->schedule(schedule_selector(DialogLayer::wordUpdate), 0.02f);

					}
					else if (ss.compare("close") == 0) {
						index = pos + l;
						pos = str.find(delim, index);

						//�浵�ı�����
						dialogNumber = dialogNumber + 1;
						getNumber(dialogNumber);

						index = pos + l;
						pos = str.find(delim, index);
						this->removeChildByTag(2);
						this->removeChildByTag(3);
						auto nowScene = Director::getInstance()->getRunningScene();
						nowScene->removeChildByTag(2);
					}
					index = pos + l;
					pos = str.find(delim, index);

					////�ͷ��ڴ�  
					//if (len > 0 && data) delete[] data;
				}
			};
		}
		//�����̶��Ի�
		else {
			//���öԻ��򱳾�
			setDialogBackground("dialog2.png");

			string filename2 = object + ".txt";
			log(filename2.c_str());
			//����ļ���ϵͳ�ľ���·��
			string filepath2 = CCFileUtils::sharedFileUtils()->fullPathForFilename(filename2);
			log(filepath2.c_str());
			//��ȡ���ֽ�������ȡʧ����Ϊ0
			ssize_t len2 = 0;
			//��ȡ������
			unsigned char *data2 = CCFileUtils::sharedFileUtils()->getFileData(filepath2.c_str(), "rb", &len2);
			string str2 = (char*)data2;
			log(str2.c_str());

			listener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {
				log("key pressed");
				keys[keyCode] = true;
				pos2 = str2.find(delim, index2);
				index2 = pos2 + l;
				pos2 = str2.find(delim, index2);
				ss = str2.substr(index2, pos2 - index2);

				if (keyCode == EventKeyboard::KeyCode::KEY_SPACE) {
					if ((ss.compare("dialog")) == 0) {
						index2 = pos2 + l;
						pos2 = str2.find(delim, index2);
						//��������
						auto portrait = Sprite::create(object + ".png");
						portrait->setTag(3);
						portrait->setAnchorPoint(Vec2(0.5, 0.5));
						portrait->setPosition(ccp(winSize.width / 3, winSize.height*0.618));
						this->removeChildByTag(3);
						this->addChild(portrait, -1);

						//��ȡ����ʾ�Ի���
						index2 = pos2 + l;
						pos2 = str2.find(delim, index2);
						ss = str2.substr(index2, pos2 - index2);
						setDialogist(ss);

						//��ȡ����ʾ�Ի�����
						index2 = pos2 + l;
						pos2 = str2.find(delim, index2);
						ss = str2.substr(index2, pos2 - index2);

						DialogLayer::i = 0;
						this->schedule(schedule_selector(DialogLayer::wordUpdate), 0.02f);

					}
					else if (ss.compare("close") == 0) {
						this->removeChildByTag(2);
						this->removeChildByTag(3);
						auto nowScene = Director::getInstance()->getRunningScene();
						nowScene->removeChildByTag(2);
					}
					index2 = pos2 + l;
					pos2 = str2.find(delim, index2);
				}
			};
		}
	}

	listener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event) {
		log("key release");
		keys[keyCode] = false;
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, m_bgSprite);

	return bRet;
}



//�����ı�����
void DialogLayer::setNumber(int i) {
	dialogNumber = i;
}

//�浵�ı�����
int DialogLayer::getNumber(int dialogNumber) {
	return dialogNumber;
}

//���öԻ���
void DialogLayer::setDialogBackground(string pictureName) {
	//���öԻ���ı���ͼƬ
	CCSprite * background = CCSprite::create(pictureName);
	m_bgSprite = background;
	CCSize contentSize = background->getContentSize();
	m_size = contentSize;
	winSize = CCDirector::sharedDirector()->getWinSize();
	m_bgSprite->setScale(1.4);
	m_bgSprite->setScaleY(1.6);
	background->setAnchorPoint(Vec2(0.5, 0));
	background->setPosition(Vec2(winSize.width, winSize.height / 15));
	background->setTag(2);
	this->addChild(background);
}

//��ʾ�Ի���
void DialogLayer::setDialogist(string ss)
{
	CCLabelTTF * content1 = CCLabelTTF::create(ss + ":", "", 24);
	content1->setAnchorPoint(Vec2(0.5, 0.5));
	content1->setPosition(ccp(m_size.width / 2 + 14, m_size.height / 2 - 7));
	//����ttf���ı���
	content1->setDimensions(CCSize(this->m_size.width, this->m_size.height));
	//����ttf��ˮƽ���뷽ʽ
	content1->setHorizontalAlignment(kCCTextAlignmentLeft);
	content1->setTag(1);
	m_bgSprite->removeChildByTag(1);
	m_bgSprite->addChild(content1);
}

//ˢ�£�������ʾ�ı�
void DialogLayer::wordUpdate(float dt)
{
	string subss = ss.substr(0, i);
	CCLabelTTF * content2 = CCLabelTTF::create(subss, "", 20);
	content2->setAnchorPoint(Vec2(0.5, .5));
	content2->setPosition(ccp(m_size.width/2, m_size.height/5)); 
	//����ttf���ı���
	content2->setDimensions(CCSize(this->m_size.width, this->m_size.height)*0.95);
	//����ttf��ˮƽ���뷽ʽ
	content2->setHorizontalAlignment(kCCTextAlignmentLeft);

	//�����ȴﵽ�����ı�����ʱʱ����ˢ��,ȡ��wordUpdate����  
	if (i > ss.length()) {
		i = 0;
		this->unschedule(schedule_selector(DialogLayer::wordUpdate));
	}
	else {
		content2->setTag(2);
		m_bgSprite->removeChildByTag(2);
		m_bgSprite->addChild(content2);
		i++;
	}
}

void DialogLayer::menuCloseCallback(Ref* pSender)
{
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif

	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);
}