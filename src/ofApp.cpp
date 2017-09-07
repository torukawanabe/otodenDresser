#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFullscreen(true);
    ofSetVerticalSync(true);
    ofHideCursor();
    
    serial.listDevices();
    vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
    for (int i=0; i<deviceList.size(); i++) {
        ofSerialDeviceInfo& info = deviceList.at(i);
        string dn = info.getDeviceName();
        if (ofIsStringInString(dn, "cu.usbserial") || ofIsStringInString(dn, "cu.usbmodem")) {
            serialStatus = serial.setup(info.getDevicePath(), 9600);
            break;
        }
    }
    
    openPlayer.load("movies/open.mp4");
    openPlayer.setLoopState(ofLoopType::OF_LOOP_NONE);
    closePlayer.load("movies/close.mp4");
    closePlayer.setLoopState(ofLoopType::OF_LOOP_NONE);
    closePlayer.play();

    makePlayers[0].load("movies/mode.mov");
    makePlayers[1].load("movies/luxury.mp4");
    makePlayers[2].load("movies/organic.mp4");
    makePlayers[3].load("movies/girly.mp4");
    makePlayer = &makePlayers[0];
    
    navImg.load("images/nav_organic.png");
    timeFont.load("font/optima.ttf", 40);
    modeFont.load("font/futura.ttf", 50);
    modeString = "mode";
    
    gui.setup();
    isOn.setName("isOn");
    isOnNextShouldTrue.setName("isOnNextShouldTrue");
    arduinoSwitch.setName("arduinoSwitchState");
    gui.add(isOn);
    gui.add(isOnNextShouldTrue);
    gui.add(arduinoSwitch);
    
    if(ofIsGLProgrammableRenderer()){
        shader.load("shadersGL3/shader");
    }else{
        shader.load("shadersGL2/shader");
    }
    fbo.allocate(1080, 1920);
    canvasFbo.allocate(1080, 1920);
}

//--------------------------------------------------------------
void ofApp::update(){
    if(serialStatus){
        updateSerial();
    }
    
    infoAlpha += (infoTargetAlpha - infoAlpha) / 20;
    movieAlpha +=  (movieTargetAlpha - movieAlpha) / 20;
    
    timeString = ofGetTimestampString("%H:%M");
    
    if(isOn){
        if(onStateMode == OnStart){
            openPlayer.update();
            
            if(ofGetElapsedTimef() > modeStartTime + onStartTime){
                initOnModePresentation();
            }
        } else if (onStateMode == OnModePresentation){
            
            if(ofGetElapsedTimef() > modeStartTime + onStartTime + onModePresentationTime){
                initOnAnimation();
            }
        } else if (onStateMode == OnAnimation){
            makePlayer->update();
        }
    }else{
        if(offStateMode == OffStart){
            makePlayer->update();

            if(ofGetElapsedTimef() > modeStartTime + offStartTime){
                initOffEnd();
            }
        }else if (offStateMode == OffEnd){
            closePlayer.update();
            
            if(closePlayer.getIsMovieDone()){
                if(ofGetElapsedTimef() > modeStartTime + offStartTime + offEndTime){
                    if(movieTargetAlpha != 0){
                        movieTargetAlpha = 0;
                    }
                }

                
                if(isOnNextShouldTrue){
                    startOn();
                }
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    
    if(isOn){
        if(onStateMode == OnStart){
            canvasFbo.begin();
            ofClear(0);
            canvasFbo.end();
            
            fbo.begin();
            ofClear(0);
            
            shader.begin();
            shader.setUniformTexture("tex0", canvasFbo.getTexture(), 1);
            shader.setUniformTexture("tex1", *openPlayer.getTexturePtr(), 2);
            shader.setUniform1f("alpha0", infoAlpha / 255.0);
            shader.setUniform1f("alpha1", movieAlpha / 255.0);
            
            canvasFbo.draw(0, 0);
            shader.end();
            fbo.end();
        } else if (onStateMode == OnModePresentation){
            canvasFbo.begin();
            ofClear(0);
            ofSetColor(255, 255, 255, 255);
            timeFont.drawString(timeString, fontPos.x, fontPos.y);
            navImg.draw(navImgPos);
            modeFont.drawString(modeString, ofGetWidth()/2 - 100, ofGetHeight()/2);
            canvasFbo.end();
            
            fbo.begin();
            ofClear(0);
            
            shader.begin();
            shader.setUniformTexture("tex0", canvasFbo.getTexture(), 1);
            shader.setUniformTexture("tex1", *openPlayer.getTexturePtr(), 2);
            shader.setUniform1f("alpha0", infoAlpha / 255.0);
            shader.setUniform1f("alpha1", movieAlpha / 255.0);

            
            canvasFbo.draw(0, 0);
            shader.end();
            fbo.end();
        } else if (onStateMode == OnAnimation){
            canvasFbo.begin();
            ofClear(0);
            ofSetColor(255, 255, 255, 255);
            timeFont.drawString(timeString, fontPos.x, fontPos.y);
            navImg.draw(navImgPos);
            canvasFbo.end();
            
            fbo.begin();
            ofClear(0);
            
            shader.begin();
            shader.setUniformTexture("tex0", canvasFbo.getTexture(), 1);
            shader.setUniformTexture("tex1", *makePlayer->getTexturePtr(), 2);
            shader.setUniform1f("alpha0", infoAlpha / 255.0);
            shader.setUniform1f("alpha1", movieAlpha / 255.0);
            
            canvasFbo.draw(0, 0);
            shader.end();
            fbo.end();
        }
    }else{
        if(offStateMode == OffStart){
            canvasFbo.begin();
            ofClear(0);
            ofSetColor(255, 255, 255, 255);
            timeFont.drawString(timeString, fontPos.x, fontPos.y);
            navImg.draw(navImgPos);
            canvasFbo.end();
            
            fbo.begin();
            ofClear(0);
            
            shader.begin();
            shader.setUniformTexture("tex0", canvasFbo.getTexture(), 1);
            shader.setUniformTexture("tex1", *makePlayer->getTexturePtr(), 2);
            shader.setUniform1f("alpha0", infoAlpha / 255.0);
            shader.setUniform1f("alpha1", movieAlpha / 255.0);

            canvasFbo.draw(0, 0);
            shader.end();
            fbo.end();
        }else if (offStateMode == OffEnd){
            canvasFbo.begin();
            ofClear(0);
            canvasFbo.end();
            
            fbo.begin();
            ofClear(0);
            
            shader.begin();
            shader.setUniformTexture("tex0", canvasFbo.getTexture(), 1);
            shader.setUniformTexture("tex1", *closePlayer.getTexturePtr(), 2);
            shader.setUniform1f("alpha0", infoAlpha / 255.0);
            shader.setUniform1f("alpha1", movieAlpha / 255.0);
            
            canvasFbo.draw(0, 0);
            shader.end();
            fbo.end();
        }
    }
    
    ofSetColor(255);
    fbo.draw(0, 0);
    
    if(debugMode){
        ofSetColor(255);
        gui.draw();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key == 's'){
        if(isOn) startOff();
        else isOnNextShouldTrue = true;
    }else if(key == 'd'){
        debugMode = !debugMode;
    }else if(key == 'f'){
        ofToggleFullscreen();
    }else if(key == '1'){
        sendRotation(0);
    }else if(key == '2'){
        sendRotation(1);
    }else if(key == '3'){
        sendRotation(2);
    }else if(key == '4'){
        sendRotation(3);
    }
}

void ofApp::updateSerial(){
    if (serial.isInitialized() == false) {
        return;
    }
    
    while (serial.available() >= 1) {
        serialStatus = true;
        int byte = serial.readByte();
        if(byte > 0){
            arduinoSwitch = true;
            isOnNextShouldTrue = true;
        }else{
            arduinoSwitch = false;
            if(isOn){
                startOff();
            }
        }
    }
}

void ofApp::startOn(){
    if(isOn) return;
    
    isOn = true;
    initOnState();
}

void ofApp::startOff(){
    if(!isOn) return;
    
    isOn = false;
    isOnNextShouldTrue = false;
    initOffState();
}

void ofApp::initOnState(){
    modeStartTime = ofGetElapsedTimef();
    onStateMode = OnStart;
    
    int index = static_cast<int>(makeType);
    index++;
    if(index > 3) index = 0;
    makeType = static_cast<MakeType>(index);
    
    openPlayer.setPosition(0);
    openPlayer.play();
    
    movieTargetAlpha = maxAlpha;
    movieAlpha = maxAlpha;
}

void ofApp::initOffState(){
    modeStartTime = ofGetElapsedTimef();
    offStateMode = OffStart;
    
    movieTargetAlpha = minAlpha;
    infoTargetAlpha = minAlpha;
}

void ofApp::initOnModePresentation(){
    onStateMode = OnModePresentation;
    switch (makeType) {
        case MakeMode:
            initModeType();
            break;
        case MakeGirly:
            initGirlyType();
            break;
        case MakeLuxury:
            initLuxuryType();
            break;
        case MakeOrganic:
            initOrganicType();
            break;
        default:
            break;
    }
    
    infoAlpha = minAlpha;
    infoTargetAlpha = maxAlpha;
}

void ofApp::initOnAnimation(){
    onStateMode = OnAnimation;
    
    movieAlpha = minAlpha;
    movieTargetAlpha = maxAlpha;
}

void ofApp::initOffEnd(){
    offStateMode = OffEnd;
    
    closePlayer.play();
    closePlayer.setPosition(0);
    closePlayer.update();
    
    movieAlpha = 255;
    movieTargetAlpha = 255;
}

void ofApp::initModeType(){
    modeString = "mode";
    navImg.load("images/nav_mode.png");
    initMakePlayer(0);
    
    sendRotation(0);
}

void ofApp::initLuxuryType(){
    modeString = "luxury";
    navImg.load("images/nav_luxury.png");
    initMakePlayer(1);

    sendRotation(1);
}

void ofApp::initOrganicType(){
    modeString = "organic";
    navImg.load("images/nav_organic.png");
    initMakePlayer(2);

    sendRotation(2);
}

void ofApp::initGirlyType(){
    modeString = "girly";
    navImg.load("images/nav_girly.png");
    initMakePlayer(3);

    sendRotation(3);
}

void ofApp::initMakePlayer(int mode){
    makePlayer = &makePlayers[mode];
    makePlayer->setPosition(0);
    makePlayer->setLoopState(ofLoopType::OF_LOOP_NORMAL);
    makePlayer->play();
    makePlayer->update();
}

void ofApp::sendRotation(int mode){
    if(serial.isInitialized()){
        serial.writeByte(mode);
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
