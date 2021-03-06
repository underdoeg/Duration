//
//  DurationController.h
//  Duration
//
//  Duration is an application for time.
//  Made at YCAM InterLab
//
//

#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxTimeline.h"
#include "ofxMSATimer.h"
#include "ofxTLUIHeader.h"
#include "ofxUI.h"
#include "ofxLocalization.h"
#include "ofxFTGLFont.h"

#ifdef TARGET_OSX
#include "ofxTLAudioTrack.h"
#endif

typedef struct {
    string path; //full project path
    string name;
    string settingsPath;
    
    bool useBPM;
    float bpm;
    bool snapToBPM;
    bool snapToKeys;
    
	float oscRate; // BUNDLES PER SECOND
    bool oscInEnabled;
	bool oscOutEnabled;
	int oscInPort;
    string oscIP;
    int oscOutPort;
    
} DurationProjectSettings;

typedef struct{
	ofRectangle sourceRect;
	ofVec2f displayPoint;
	string text;
	ofColor debugColor;
} Tooltip;

class DurationController : public ofThread  {
  public:
	DurationController();
	~DurationController();
	
	void setup();
		
	void enableInterface();
	void disableInterface();
	bool isInterfaceEnabled();
	
	void update(ofEventArgs& args);
	void draw(ofEventArgs& args);
	
	void keyPressed(ofKeyEventArgs& keyArgs);
		
	ofxTimeline& getTimeline();
	
    ofRectangle canvasRectangle;
    ofxUICanvas* gui;
	void guiEvent(ofxUIEventArgs& e);
    void exit(ofEventArgs& e);
	
	ofxTLTrack* addTrack(string trackType, string trackName = "", string xmlFileName = "");
	
    //control elements
    ofxUIDropDownList* projectDropDown;
	ofxUIMultiImageButton* saveButton;
    ofxUILabel* timeLabel;
    ofxUITextInput* durationLabel;
    ofxUIMultiImageToggle* playpauseToggle;
	ofxUIMultiImageButton* stopButton;
    ofxUIMultiImageToggle* loopToggle;
	
	//project settings elements
    ofxUILabelToggle* useBPMToggle;
	ofxUINumberDialer* bpmDialer;
	
	//TODO: find a place for these
    ofxUILabelToggle* snapToKeysToggle;
    
    ofxUILabelToggle* enableOSCInToggle;
	ofxUILabelToggle* enableOSCOutToggle;
    ofxUITextInput* oscInPortInput;
    ofxUITextInput* oscOutIPInput;
    ofxUITextInput* oscOutPortInput;
    
    vector<string> trackTypes;
    ofxUIDropDownList* addTrackDropDown;
	
    string defaultProjectDirectoryPath;
	DurationProjectSettings settings;
	
	void newProject(string projectPath);
    void newProject(string newProjectPath, string newProjectName);
    void loadProject(string projectPath, bool forceCreate = false);
    void loadProject(string projectPath, string projectName, bool forceCreate = false);
    void saveProject();
    
    DurationProjectSettings defaultProjectSettings();
    	
	void startRecording();
	void stopRecording();

  protected:
	ofxTimeline timeline;
    void bangFired(ofxTLBangEventArgs& bang);
	vector<string> trackAddresses;
	
	bool shouldStartPlayback;
	void startPlayback();
	void sendInfoMessage();
	bool refreshAllOscOut;
	
    bool shouldCreateNewProject;
	string newProjectPath;
    bool shouldLoadProject;
	string projectToLoad;
	
	ofxOscReceiver receiver;
	ofxOscSender sender;
	
	void threadedFunction();
	void handleOscOut();
	void handleOscIn();
	bool enabled;
	
	unsigned long recordTimeOffset;
	ofxMSATimer recordTimer;

	void createTooltips();
	void drawTooltips();
	void drawTooltipDebug();
	
	ofxLocalization translation;
	ofMutex oscLock;
	
	vector<Tooltip> tooltips;
	#ifdef TARGET_OSX
	//only can have one of these!
	ofxTLAudioTrack* audioTrack;
	#endif

	vector<ofxOscMessage> bangsReceived;
	map<string, ofPtr<ofxTLUIHeader> > headers;
    
	ofxTLUIHeader* createHeaderForTrack(ofxTLTrack* track);
	ofPtr<ofxTLUIHeader> getHeaderWithDisplayName(string name);
	
	float oscFrequency; // 1 / BUNDLES PER SECOND * 1000
	unsigned long lastOSCBundleSent;
	ofxFTGLFont tooltipFont;
	bool needsSave;
	
	bool receivedAddTrack;
	string oscTrackTypeReceived;
	string oscTrackNameReceived;
	string oscTrackFilePathReceived;
	
	bool receivedPaletteToLoad;
	ofxTLColorTrack* paletteTrack;
	string palettePath;
	
};

