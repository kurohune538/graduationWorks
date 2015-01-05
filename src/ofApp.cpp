/*
 * This is a simple example use of ofArduino
 *
 * ofArduino currently only supports the standard Arduino boards
 * (UNO, Duemilanove, Diecimila, NG, and other boards based on the
 * ATMega168 or ATMega328 microcontrollers
 * The Arduio FIO and Arduino Mini should also work.
 * The Arduino MEGA and other variants based on microcontrollers
 * other than the ATMega168 and ATMega328 are not currently supported.
 * 
 * To use this example, open Arduino (preferably Arduino 1.0) and 
 * navigate to File -> Examples -> Firmata and open StandardFirmata.
 * Compile and upload StandardFirmata for your board, then close
 * the Arduino application and run this application.
 *
 * If you have a servo attached, press the left arrow key to rotate
 * the servo head counterclockwise and press the right arrow key to
 * rotate the servo head clockwise.
 *
 * Clicking the mouse over any part of the application will turn the
 * on-board LED on and off.
 *
 */

#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){

	ofSetVerticalSync(true);
	ofSetFrameRate(60);

	ofBackground(255,255,255);
    
    buttonState = "digital pin:";
    potValue0 = "analog pin:";

    //背景のArduino画像の表示
	//bgImage.loadImage("background.png");
    
    //fontの指定とサイズ
	font.loadFont("franklinGothic.otf", 10);
    smallFont.loadFont("franklinGothic.otf", 14);

    // replace the string below with the serial port for your Arduino board
    // you can get this from the Arduino application or via command line
    // for OSX, in your terminal type "ls /dev/tty.*" to get a list of serial devices
    //Arduinoのシリアルポートを選択し接続
	ard.connect("/dev/tty.usbmodem1421", 57600);
	
	// listen for EInitialized notification. this indicates that
	// the arduino is ready to receive commands and it is safe to
	// call setupArduino()
	ofAddListener(ard.EInitialized, this, &ofApp::setupArduino);
	bSetupArduino	= false;	// flag so we setup arduino when its ready, you don't need to touch this :)
    
    
    //以下box
    ofEnableDepthTest();
    
    ofEnableNormalizedTexCoords();
    //ofのロゴをロード
    //ofLogo.loadImage("inaginasi06_color_tri.jpg");
    //boxの枠線のウエイトを設定
    ofSetLineWidth(10);
    
    isPointFinger = false;
}



//--------------------------------------------------------------
void ofApp::update(){

	updateArduino();
    //加速度のデータが存在するかどうか
    if( ard.getAnalog(5) && ard.getAnalog(6) && ard.getAnalog(7)){
        data.push_back(serial.readByte());
        
        if ( data.size() > ofGetWidth() ){
            data.erase(data.begin());
        }
    }
    

}

//--------------------------------------------------------------
void ofApp::setupArduino(const int & version) {
	
	// remove listener because we don't need it anymore
	ofRemoveListener(ard.EInitialized, this, &ofApp::setupArduino);
    
    // it is now safe to send commands to the Arduino
    bSetupArduino = true;
    
    // print firmware name and version to the console
    ofLogNotice() << ard.getFirmwareName(); 
    ofLogNotice() << "firmata v" << ard.getMajorFirmwareVersion() << "." << ard.getMinorFirmwareVersion();
        
    // Note: pins A0 - A5 can be used as digital input and output.
    // Refer to them as pins 14 - 19 if using StandardFirmata from Arduino 1.0.
    // If using Arduino 0022 or older, then use 16 - 21.
    // Firmata pin numbering changed in version 2.3 (which is included in Arduino 1.0)
    
    // set pins D2 and A5 to digital input
    ard.sendDigitalPinMode(2, ARD_INPUT);
    ard.sendDigitalPinMode(19, ARD_INPUT);  // pin 21 if using StandardFirmata from Arduino 0022 or older

    // set pin A0 to analog input
    //A0からA4までのアナログピンを送信
    //曲げセンサ
    ard.sendAnalogPinReporting(0, ARD_ANALOG);
    ard.sendAnalogPinReporting(1, ARD_ANALOG);
    ard.sendAnalogPinReporting(2, ARD_ANALOG);
    ard.sendAnalogPinReporting(3, ARD_ANALOG);
    ard.sendAnalogPinReporting(4, ARD_ANALOG);
    //３軸加速度センサ
    ard.sendAnalogPinReporting(5, ARD_ANALOG);
    ard.sendAnalogPinReporting(6, ARD_ANALOG);
    ard.sendAnalogPinReporting(7, ARD_ANALOG);

    // set pin D13 as digital output
	ard.sendDigitalPinMode(13, ARD_OUTPUT);
    // set pin A4 as digital output
    ard.sendDigitalPinMode(18, ARD_OUTPUT);  // pin 20 if using StandardFirmata from Arduino 0022 or older

    // set pin D11 as PWM (analog output)
	ard.sendDigitalPinMode(11, ARD_PWM);
    
    // attach a servo to pin D9
    // servo motors can only be attached to pin D3, D5, D6, D9, D10, or D11
    ard.sendServoAttach(9);
	
    // Listen for changes on the digital and analog pins
    ofAddListener(ard.EDigitalPinChanged, this, &ofApp::digitalPinChanged);
    ofAddListener(ard.EAnalogPinChanged, this, &ofApp::analogPinChanged);
    
    }

//--------------------------------------------------------------
void ofApp::updateArduino(){

	// update the arduino, get any data or messages.
    // the call to ard.update() is required
	ard.update();
	
	// do not send anything until the arduino has been set up
	if (bSetupArduino) {
        // fade the led connected to pin D11
		ard.sendPwm(11, (int)(128 + 128 * sin(ofGetElapsedTimef())));   // pwm...
	}

}

// digital pin event handler, called whenever a digital pin value has changed
// note: if an analog pin has been set as a digital pin, it will be handled
// by the digitalPinChanged function rather than the analogPinChanged function.

//--------------------------------------------------------------
void ofApp::digitalPinChanged(const int & pinNum) {
    // do something with the digital input. here we're simply going to print the pin number and
    // value to the screen each time it changes
    buttonState = "digital pin: " + ofToString(pinNum) + " = " + ofToString(ard.getDigital(pinNum));
}

// analog pin event handler, called whenever an analog pin value has changed

//--------------------------------------------------------------
void ofApp::analogPinChanged(const int & pinNum) {
    // do something with the analog input. here we're simply going to print the pin number and
    // value to the screen each time it changes
    //potValue = "analog pin: " + ofToString(pinNum) + " = " + ofToString(ard.getAnalog(pinNum));
    //曲げセンサ
    potValue0 = "analog pin: " + ofToString(0) + " = " + ofToString(ard.getAnalog(0));
    potValue1 = "analog pin: " + ofToString(1) + " = " + ofToString(ard.getAnalog(1));
    potValue2 = "analog pin: " + ofToString(2) + " = " + ofToString(ard.getAnalog(2));
    potValue3 = "analog pin: " + ofToString(3) + " = " + ofToString(ard.getAnalog(3));
    potValue4 = "analog pin: " + ofToString(4) + " = " + ofToString(ard.getAnalog(4));
    
    //３軸加速度センサ
    potValue5 = "analog pin: " + ofToString(5) + " = " + ofToString(ard.getAnalog(5));
    potValue6 = "analog pin: " + ofToString(6) + " = " + ofToString(ard.getAnalog(6));
    potValue7 = "analog pin: " + ofToString(7) + " = " + ofToString(ard.getAnalog(7));


}


//--------------------------------------------------------------
void ofApp::draw(){
	//bgImage.draw(0,0);
    
    ofBackgroundGradient(ofColor(60,60,60), ofColor(10,10,10));
    ofEnableAlphaBlending();
    
    
    //ofSetColor(190);
    
    ofSetColor(0, 0, 0, 127);
    //ofRect(800, 15, 200, 150);
    ofDisableAlphaBlending();
    
    ofSetColor(255, 255, 255);
	if (!bSetupArduino){
		font.drawString("arduino not ready...\n", 805, 40);
	} else {
        ofSetColor(255, 255, 255);
		font.drawString(potValue0 + "\n" + potValue1 + "\n" + potValue2 + "\n" + potValue3 + "\n" + potValue4 + "\n" + potValue5 + "\n" +potValue6 + "\n" +potValue7 + "\n" +buttonState +
						"\nsending pwm: " + ofToString((int)(128 + 128 * sin(ofGetElapsedTimef()))), 805, 40);
        
        //3軸加速度のゲージ
        ofSetColor(100, 100, 100);
        ofFill();
        ofRect(0, 0, ard.getAnalog(5)-450, 50);
        
        ofSetColor(150, 150, 150);
        ofFill();
        ofRect(0, 50, ard.getAnalog(6)-450, 50);
        
        ofSetColor(200, 200, 200);
        ofFill();
        ofRect(0, 100, ard.getAnalog(7)-450, 50);
        
        //グラフ
        
//        ofBackground(100);
//        ofNoFill();
//        ofBeginShape();
//        for(int i = 0; i < data.size(); i++ ){
//            ofVertex((data.size()-i), data[i]+ofGetHeight()/2);
//        }
//        ofEndShape();
        
        //xとｚがあがるとダイス
        //ｘ４００ｙ５５０ｚ５１０→ｚ４００→x６２０ｙ４４０ｚ５２０
        //        smallFont.drawString("If a servo is attached, use the left arrow key to rotate "
//                             "\ncounterclockwise and the right arrow key to rotate clockwise.", 200, 550);
        //ofSetColor(255, 255, 255);

        
        //以下box------------------------------------------------------------------------------------
//        float movementSpeed = 0.1;
//        float cloudSize = ofGetWidth() / 2;
//        float maxBoxSize = 100;
//        float spacing = 1;
//        float boxCount = 100;
//        float t;
//        float t2;
//        float t3;
//        cam.begin();
//        
//        for(int i = 0; i < boxCount; i++) {
//            ofPushMatrix();
//            
//            
//            //ポイントしていない時に図形を動かす
//            if(!isPointFinger){
//                //cout<< "tの値更新中"<<endl;
//                t = (ofGetElapsedTimef() + i * spacing) * movementSpeed;
//                
//                t2 = (ofGetElapsedTimef() + i * spacing) * movementSpeed;
//                
//            
//                //cout << "t:" << t << " "<< "t2:" << t2 << endl;
//                
//            }
//            
//            ofVec3f pos(
//                        ofSignedNoise(t, 0, 0),
//                        ofSignedNoise(0, t, 0),
//                        ofSignedNoise(0, 0, t));
//
//            
//            ofVec3f pos2(
//                         ofSignedNoise(t2, 0, 0),
//                         ofSignedNoise(0, t2, 0),
//                         ofSignedNoise(0, 0, t2));
//            
//            //曲げの平均値を取る
//            float ardAverage = ard.getAnalog(0) + ard.getAnalog(1) + ard.getAnalog(2) + ard.getAnalog(3) + ard.getAnalog(4) / 5;
//            
//            //float boxSize = maxBoxSize * ofNoise(pos.x, pos.y, pos.z);
//            //曲げ具合によってboxの大きさを変化
//            float boxSize = maxBoxSize * ardAverage * 0.0005;
//            
//            
//            //ポイントしてるときだけ図形が止まる
//            if(ard.getAnalog(0) < 500 && ard.getAnalog(2) < 500 && ard.getAnalog(3) < 500 && ard.getAnalog(4) < 500 && ard.getAnalog(1) > 480 && ard.getAnalog(1) < 510){
//                isPointFinger = true;
//                //cout<<"ポイント中"<<endl;
//                t3 = t2;
//                pos2.x = ofSignedNoise(t3,0,0);
//                pos2.y = ofSignedNoise(0,t3,0);
//                pos2.z = ofSignedNoise(0,0,t3);
//                cout<<"t3:"<< t3 <<endl;
//                //pos2 *= cloudSize;
//                
//                ofTranslate(pos2);
//                ofRotateX(pos2.x);
//                ofRotateY(pos2.y);
//                ofRotateZ(pos2.z);
//                
//                ofLogo.bind();
//                ofFill();
//                ofSetColor(ard.getAnalog(0) / 2, ard.getAnalog(1) /2, ard.getAnalog(2) /2);
//                ofDrawBox(boxSize);
//                ofLogo.unbind();
//                
//                
//                ofNoFill();
//                ofSetColor(ofColor::fromHsb(sinf(t) * 128 + 128, 255, 255));
//                ofDrawBox(boxSize * 1.1f);
//                
//            }else{
//                isPointFinger = false;
//                //cout<<"ポイント解除"<<endl;
//                
//                pos *= cloudSize;
//                pos2 *= cloudSize;
//                ofTranslate(pos);
//
//                ofRotateX(pos2.x);
//                ofRotateY(pos2.y);
//                ofRotateZ(pos2.z);
//                
//                ofLogo.bind();
//                ofFill();
//                ofSetColor(ard.getAnalog(0) / 2, ard.getAnalog(1) /2, ard.getAnalog(2) /2);
//                ofDrawBox(boxSize);
//                ofLogo.unbind();
//                
//                
//                ofNoFill();
//                ofSetColor(ofColor::fromHsb(sinf(t) * 128 + 128, 255, 255));
//                ofDrawBox(boxSize * 1.1f);
//                
//            }
//
//            
//            
//            
//            
//            ofPopMatrix();
//        }
//        
//        cam.end();
    
        //Boxここまで--------------------------------------------------------------------------------------
        
	}
    
    
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
    switch (key) {
        case OF_KEY_RIGHT:
            // rotate servo head to 180 degrees
            ard.sendServo(9, 180, false);
            ard.sendDigital(18, ARD_HIGH);  // pin 20 if using StandardFirmata from Arduino 0022 or older
            break;
        case OF_KEY_LEFT:
            // rotate servo head to 0 degrees
            ard.sendServo(9, 0, false);
            ard.sendDigital(18, ARD_LOW);  // pin 20 if using StandardFirmata from Arduino 0022 or older
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    // turn on the onboard LED when the application window is clicked
	ard.sendDigital(13, ARD_HIGH);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    // turn off the onboard LED when the application window is clicked
	ard.sendDigital(13, ARD_LOW);
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