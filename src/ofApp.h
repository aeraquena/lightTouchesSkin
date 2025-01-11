#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxGui.h"
#include "ofxDelaunay.h"

class ofApp : public ofBaseApp {
public:
    
    /*******************
     * MARK: Functions *
     *******************/
    
    void setup();
    void update();
    void draw();
    void exit();
    void keyPressed(int key);
    vector <ofPoint> getTriangle(int i, ofxDelaunay thisTriangulation);
    void updateTargets();
    
    /*****************
     * MARK: Structs *
     *****************/
    
    // Shape color
    typedef struct {
        ofColor light;
        ofColor dark;
        ofColor dot;
    } shapeColor;
    
    // Target triangle
    typedef struct {
        ofPoint point1;
        ofPoint point2;
        ofPoint point3;
        bool touching;
        int boundingXMin;
        int boundingYMin;
        int boundingXMax;
        int boundingYMax;
    } targetTriangle;
    
    /********************
     * MARK: Resolution *
     ********************/
    
    // Projection
    int PROJECTION_WIDTH;
    int PROJECTION_HEIGHT;
    
    // Laptop screen
    int SCREEN_WIDTH = 1440;
    int SCREEN_HEIGHT = 900;
    
    // Scale value of projection resolution to Kinect resolution
    int scaleVal;
    
    /****************
     * MARK: Kinect *
     ****************/
    
    ofxKinect kinect;
    
    // Kinect images
    ofxCvColorImage colorImg; // color video image
    ofxCvGrayscaleImage grayImage; // grayscale depth image
    ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
    ofxCvGrayscaleImage grayThreshFar; // the far thresholded image
    
    ofxCvContourFinder contourFinder;
    
    // Movable rectangle to calculate region of interest / subsection of original image
    ofxIntSlider roiX;
    ofxIntSlider roiY;
    ofxIntSlider roiW;
    ofxIntSlider roiH;
    
    // Depth thresholds
    ofxIntSlider nearThreshold;
    ofxIntSlider minFarThreshold;
    ofxIntSlider maxFarThreshold;
    
    // Blob properties
    ofxIntSlider minBlobArea;
    ofxIntSlider maxBlobArea;
    ofxIntSlider maxBlobNum;
    
    ofxIntSlider onePersonBlobArea;
    float blobArea;
    int prevNumBlobs = 0;
    
    /**************
     * MARK: FBOs *
     **************/
    
    // Final FBO combining all body shapes to output onto projection
    ofFbo finalFbo;
    
    // Shape FBOs
    vector<ofFbo> shapeFbos;
    
    // Texture FBOs
    vector<ofFbo> textureFbos;
    
    // Target FBO
    ofFbo targetFbo;
    
    // Max number of shape FBOs
    const int NUM_SHAPE_FBOS = 6;
    
    // Position of shape and target FBOs
    ofxIntSlider fboLeft;
    ofxIntSlider fboTop;
    
    // Additional position of shape FBO
    ofxIntSlider shapeFboTop;
    ofxIntSlider shapeFboLeft;
    
    /******************
     * MARK: Graphics *
     ******************/
    
    // Target triangle line width
    ofxIntSlider lineWidth;
    
    // Smoothing values for blobs
    ofxIntSlider smoothingSize;
    ofxFloatSlider smoothingShape;
    ofxIntSlider blurValue;
    ofxIntSlider blurThreshold;
    
    // Colors
    vector<shapeColor> shapeColors;
    
    /*****************
     * MARK: Targets *
     *****************/
    
    // Vectors of target points
    vector<ofPoint> prevTargets;
    vector<ofPoint> nextTargets;
    float targetLerpPercent = 1.;
    
    // Target values
    bool moveTarget;
    
    // Target triangles
    ofxDelaunay triangulation;
    ofxDelaunay nextTriangulation;
    ofxDelaunay triangulation2;
    ofxDelaunay nextTriangulation2;
    vector<targetTriangle> targetTriangles;
    vector<ofPoint> targetPoints;
    
    // Number of points per triangulation
    ofxIntSlider numTargets;
    
    // Number of pixels per real inch
    const float INCH_PIXELS = 12.5;
    
    // Dividing axis between two triangulated shapes
    int dividingAxis;
    
    // Minimum height of dividing line
    int minDividingHeight = 5 * INCH_PIXELS;
    
    // Minimum distance between dividing line and triangulated shapes
    int minTriangleUnit = 1 * INCH_PIXELS;
    
    // Brightness of touched triangle
    const float TOUCHING_TRIANGLE_BRIGHTNESS = 168;
    
    // Are all targets being touched?
    bool allTargetsTouching;
    
    // Is this the first time drawing either triangulated shape?
    bool firstTime = true;
    bool firstTime2ndShape = true;
    
    // Max dimensions of play area in inches
    ofxIntSlider maxInchesX;
    ofxIntSlider maxInchesY;
    const int TOTAL_MAX_INCHES_X = 128;
    const int TOTAL_MAX_INCHES_Y = 80;
    
    // Are there two target shapes?
    bool twoTargets;
    
    // Number of triangles touching
    int numTrianglesTouching;
    
    // Frames to delay before changing between 1 and 2 shapes
    ofxIntSlider changeShapeDelay;
    
    /**************
     * MARK: Text *
     **************/
    
    // Font
    ofTrueTypeFont franklinBook;
    
    // Text position
    int textTop;
    int textLeft;
    int textRight;
    int textTop2;
    float textRotate;
    float textRotate2;
    
    /*************
     * MARK: GUI *
     *************/
    
    // Display modes
    int roomMode;
    int videoMode;
    
    // GUI
    ofxPanel gui;
    bool bHide;
    
    /**********************
     * MARK: Target tiles *
     **********************/
    
    // Tile shape indices
    int tileShapeNum = 0;
    int tileShapeNum2 = 0;
    
    // Size per tile in shape
    int blockSize;
    
    // Possible tile shapes
    
    const int NUM_TILE_SHAPES = 53;
    
    int tileShapes[53][3][3] = {
        { // L
            {1,0,0},
            {1,0,0},
            {1,1,1}
        },
        { // L
            {1,1,1},
            {1,0,0},
            {1,0,0}
        },
        { // L
            {1,1,1},
            {0,0,1},
            {0,0,1}
        },
        { // L
            {0,0,1},
            {0,0,1},
            {0,0,1}
        },
        { // W
            {1,0,0},
            {1,1,0},
            {0,1,1}
        },
        { // W
            {0,1,1},
            {1,1,0},
            {1,0,0}
        },
        { // W
            {1,1,0},
            {0,1,1},
            {0,0,1}
        },
        { // W
            {0,0,1},
            {0,1,1},
            {1,1,0}
        },
        { // I - middle
            {0,1,0},
            {0,1,0},
            {0,1,0}
        },
        { // I - middle
            {0,0,0},
            {1,1,1},
            {0,0,0}
        },
        { // I - edge
            {1,0,0},
            {1,0,0},
            {1,0,0}
        },
        { // I - edge
            {1,1,1},
            {0,0,0},
            {0,0,0}
        },
        { // I - edge
            {0,0,0},
            {0,0,0},
            {1,1,1}
        },
        { // I - edge
            {0,0,1},
            {0,0,1},
            {0,0,1}
        },
        { // 4
            {1,1,0},
            {1,1,0},
            {0,0,0}
        },
        { // 4
            {0,1,1},
            {0,1,1},
            {0,0,0}
        },
        { // 4
            {0,0,0},
            {0,1,1},
            {0,1,1}
        },
        { // 4
            {0,0,0},
            {1,1,0},
            {1,1,0}
        },
        { // 6
            {1,1,0},
            {1,1,0},
            {1,1,0}
        },
        { // 6
            {0,1,1},
            {0,1,1},
            {0,1,1}
        },
        { // 6
            {1,1,1},
            {1,1,1},
            {0,0,0}
        },
        { // 6
            {0,0,0},
            {1,1,1},
            {1,1,1}
        },
        { // F
            {0,1,1},
            {1,1,0},
            {0,1,0}
        },
        { // F
            {0,1,1},
            {1,1,0},
            {0,1,0}
        },
        { // F flip
            {1,1,0},
            {0,1,1},
            {0,1,0}
        },
        { // Small L
            {1,0,0},
            {1,0,0},
            {1,1,0}
        },
        { // Small L
            {1,1,1},
            {0,0,1},
            {0,0,0}
        },
        { // Small L
            {0,0,1},
            {0,0,1},
            {0,1,1}
        },
        { // Small L
            {0,0,0},
            {1,0,0},
            {1,1,1}
        },
        { // Small L flipped
            {0,0,1},
            {0,0,1},
            {0,1,1}
        },
        { // Small L flipped
            {0,0,0},
            {1,0,0},
            {1,1,1}
        },
        { // Small L flipped
            {1,1,0},
            {1,0,0},
            {1,0,0}
        },
        { // Small L flipped
            {1,1,1},
            {0,0,1},
            {0,0,0}
        },
        { // b
            {1,1,0},
            {1,1,0},
            {1,0,0}
        },
        { // b flipped
            {0,1,1},
            {0,1,1},
            {0,0,1}
        },
        { // X
            {0,1,0},
            {1,1,1},
            {0,1,0}
        },
        { // T
            {1,1,1},
            {0,1,0},
            {0,1,0}
        },
        { // T
            {0,1,0},
            {0,1,0},
            {1,1,1}
        },
        { // T
            {1,0,0},
            {1,1,1},
            {1,0,0}
        },
        { // T
            {0,0,1},
            {1,1,1},
            {0,0,1}
        },
        { // T small
            {1,1,1},
            {0,1,0},
            {0,0,0}
        },
        { // T
            {0,0,0},
            {0,1,0},
            {1,1,1}
        },
        { // T
            {1,0,0},
            {1,1,0},
            {1,0,0}
        },
        { // T
            {0,0,1},
            {0,1,1},
            {0,0,1}
        },
        { // Z
            {1,1,0},
            {0,1,0},
            {0,1,1}
        },
        { // Z
            {0,0,1},
            {1,1,1},
            {1,0,0}
        },
        { // Z flipped
            {0,1,1},
            {0,1,0},
            {1,1,0}
        },
        { // Z flipped
            {1,0,0},
            {1,1,1},
            {0,0,1}
        },
        { // Z small
            {1,1,0},
            {0,1,1},
            {0,0,0}
        },
        { // Z small
            {0,0,1},
            {0,1,1},
            {0,1,0}
        },
        { // Small Z flipped
            {0,0,0},
            {0,1,1},
            {1,1,0}
        },
        { // Small Z flipped
            {1,0,0},
            {1,1,0},
            {0,1,0}
        },
        { // All
            {1,1,1},
            {1,1,1},
            {1,1,1}
        }
    };
};
