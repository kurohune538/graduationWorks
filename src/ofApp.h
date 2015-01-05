#pragma once

#include "ofMain.h"
#include "ofEvents.h"

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
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	ofImage				bgImage;
	ofTrueTypeFont		font;
    ofTrueTypeFont      smallFont;
	ofArduino	ard;
	bool		bSetupArduino;			// flag variable for setting up arduino once
    ofSerial serial;
    vector<float> data;
    
    //以下box
    //ofのロゴ
    ofImage ofLogo;
    //ライトを生成しライティングを可能に
    ofLight light;
    //マウスでカメラ操作を可能に
    ofEasyCam cam;
    bool isPointFinger;
    
private:
    
    void setupArduino(const int & version);
    void digitalPinChanged(const int & pinNum);
    void analogPinChanged(const int & pinNum);
	void updateArduino();
    
    string buttonState;
    string potValue0;
    string potValue1;
    string potValue2;
    string potValue3;
    string potValue4;
    string potValue5;
    string potValue6;
    string potValue7;
    

    

};

