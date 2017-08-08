#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    void updateSerial();
    
    void startOn();
    void startOff();
    
    void initOnState();
    void initOffState();
    
    void initOnModePresentation();
    void initOnAnimation();
    void initOffEnd();
    
    void initModeType();
    void initLuxuryType();
    void initOrganicType();
    void initGirlyType();
    
    void initMakePlayer(int mode);
    
    void sendRotation(int mode);
    
    ofxPanel gui;
    ofVideoPlayer openPlayer, closePlayer;
    ofVideoPlayer* makePlayer;
    ofVideoPlayer makePlayers[4];
    
    ofImage navImg;
    ofTrueTypeFont timeFont, modeFont;
    string timeString, modeString;
    
    ofPoint navImgPos = ofPoint(224, 50);
    ofPoint fontPos = ofPoint(50, 170);
    ofPoint playerMakePos = ofPoint(0, 0);
    ofPoint playerCurtainPos = ofPoint(0, 0);
    
    ofSerial serial;
    bool serialStatus = false;
    
    float modeStartTime;
    const float onStartTime = 3.0;
    const float onModePresentationTime = 3.5;
    const float offStartTime = 1.0;
    const float offEndTime = 6.0;
    
    const float maxAlpha = 255;
    const float minAlpha = 0;
    
    float infoAlpha = 255;
    float movieAlpha = 255;
    
    float infoTargetAlpha = 255;
    float movieTargetAlpha = 255;
    
    //State
    
    ofParameter<bool> isOn = false;
    ofParameter<bool> isOnNextShouldTrue = false;
    ofParameter<bool> arduinoSwitch = false;
    ofParameter<bool> debugMode = false;
    
    enum MakeType : int {
        MakeMode,
        MakeLuxury,
        MakeOrganic,
        MakeGirly
    };
    
    enum OnStateMode {
        OnStart,
        OnModePresentation,
        OnAnimation
    };
    
    enum OffStateMode {
        OffStart,
        OffEnd
    };
    
    MakeType makeType = MakeMode;
    OnStateMode onStateMode = OnStart;
    OffStateMode offStateMode = OffEnd;
		
};
