#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    
    /****************************
     * MARK: Changing variables *
     ****************************/
    
    // Physical setup
    
    // 0 = Dual screen desktop
    // 1 = Bedroom wall
    // 2 = Single screen desktop
    // 3 = Church floor, single projection (presentation mode)
    // 4 = Church floor, dual screen and projection
    roomMode = 4;
    
    // Mode for video on first screen
    
    // 1. Video
    // 2. Depth video
    // 3. Threshold
    // 4. Contour
    videoMode = 0;
    
    // Set frame rate
    ofSetFrameRate(60);
    
    /****************
     * MARK: Kinect *
     ****************/
    
    // Enable depth->video image calibration
    kinect.setRegistration(true);
    
    kinect.init();
    
    kinect.open();
    
    // Allocate images
    colorImg.allocate(kinect.width, kinect.height);
    grayImage.allocate(kinect.width, kinect.height);
    grayThreshNear.allocate(kinect.width, kinect.height);
    grayThreshFar.allocate(kinect.width, kinect.height);
    
    /************************
     * MARK: Initialize GUI *
     ************************/
    
    gui.setup();
    
    // Scale value of projection resolution to Kinect resolution
    scaleVal = 2;
    
    // Set Kinect depth detection thresholds
    if (roomMode == 0) {
        // DO NOT MODIFY
        // Desktop
        // 4 feet away
        
        gui.add(nearThreshold.setup("near threshold", 85, 0, 255));
        
        gui.add(roiX.setup("roi x", 25, 0, 640));
        gui.add(roiY.setup("roi y", 52, 0, 480));
        gui.add(roiW.setup("roi w", 582, 0, 640));
        gui.add(roiH.setup("roi h", 420, 0, 480));
        
        // Blob area considered to be boundary between one and two people
        gui.add(onePersonBlobArea.setup("1p blob area", 18000, 8000, 35000));
        
    } else if (roomMode == 1) {
        // DO NOT MODIFY
        // Bedroom wall
        // 8 feet away
        
        gui.add(nearThreshold.setup("near threshold", 177, 0, 255));
        
        gui.add(roiX.setup("roi x", 163, 0, 640));
        gui.add(roiY.setup("roi y", 136, 0, 480));
        gui.add(roiW.setup("roi w", 294, 0, 640));
        gui.add(roiH.setup("roi h", 182, 0, 480));
        
        // Blob area considered to be threshold between one and two people
        gui.add(onePersonBlobArea.setup("1p blob area", 75000, 8000, 75000));
        
    } else if (roomMode == 2) {
        // DO NOT MODIFY
        // Desktop
        // 4 feet away
        
        gui.add(nearThreshold.setup("near threshold", 242, 0, 255));
        
        gui.add(roiX.setup("roi x", 28, 0, 640));
        gui.add(roiY.setup("roi y", 40, 0, 480));
        gui.add(roiW.setup("roi w", 612, 0, 640));
        gui.add(roiH.setup("roi h", 440, 0, 480));
        
        // Blob area considered to be boundary between one and two people
        gui.add(onePersonBlobArea.setup("1p blob area", 75000, 8000, 75000));
        
        scaleVal = 1;
    } else if (roomMode == 3 || roomMode == 4) {
        // MODIFY THIS
        // Church floor
        // 10.5 feet away
        
        gui.add(nearThreshold.setup("near threshold", 80, 0, 255));
        
        gui.add(roiX.setup("roi x", 0, 0, 80));
        gui.add(roiY.setup("roi y", 0, 0, 200));
        gui.add(roiW.setup("roi w", 640, 500, 640));
        gui.add(roiH.setup("roi h", 480, 320, 480));
        
        scaleVal = 3;
    
        // Blob area considered to be boundary between one and two people
        gui.add(onePersonBlobArea.setup("1p blob area", 200000, 8000, 300000));
    }
    
    // Set blob parameters
    if (roomMode == 2) {
        // DO NOT MODIFY
        // Minimum blob area
        gui.add(minBlobArea.setup("min blob area", 22000, 1000, 50000));
        
        gui.add(minFarThreshold.setup("min far threshold", 66, 0, 70));
        gui.add(maxFarThreshold.setup("max far threshold", 67, 0, 70));
    } else {
        // MODIFY THIS
        // Minimum blob area
        gui.add(minBlobArea.setup("min blob area", 1200, 0, 30000));
        // to account for people making holes with their hands
        
        // Depth thresholds
        gui.add(minFarThreshold.setup("min far threshold", 0, 0, 70));
        gui.add(maxFarThreshold.setup("max far threshold", 0, 0, 70));
    }
    
    // Blob values
    gui.add(maxBlobArea.setup("max blob area", roiW * roiH - 10000, 0, roiW * roiH)); // TODO: Change to 2000? //(roiW * roiH) - 4000
    // changed to 200000 to avoid the whole thing recognized as blob
    gui.add(maxBlobNum.setup("max blob num", 6, 1, 50));
    
    // Smoothing values for blobs
    gui.add(smoothingSize.setup("smoothing size", 11, 0, 100));
    gui.add(smoothingShape.setup("smoothing shape", 0, 0, 1));
    gui.add(blurValue.setup("blur value", 31, 0, 100)); // must be odd
    gui.add(blurThreshold.setup("blur threshold", 105, 0, 255));

    // Position of shape and target FBOs
    // To align silhouettes with bodies
    
    if (roomMode == 2) {
        // DO NOT MODIFY THIS
        gui.add(fboLeft.setup("fbo left", 248, -100, 300));
        gui.add(fboTop.setup("fbo top", 40, -200, 400));
        gui.add(shapeFboTop.setup("shape fbo top", 0, -332, 0));
        gui.add(shapeFboLeft.setup("shape fbo left", -45, -200, 200));
    } else {
        // MODIFY THIS
        gui.add(fboLeft.setup("fbo left", 276, -3000, 600));
        gui.add(fboTop.setup("fbo top", 130, -200, 300));
        gui.add(shapeFboTop.setup("shape fbo top", 212, -400, 500));
        gui.add(shapeFboLeft.setup("shape fbo left", 678, 500, 800));
    }
    
    gui.add(scaleVal.setup("shape scale val", 2.0, 1.0, 3.0));
    
    // Target triangle line width
    gui.add(lineWidth.setup("line width", 4, 1, 15));

    // Number of target points
    gui.add(numTargets.setup("num targets", 5, 3, 50));
    
    // Frames to delay before changing between 1 and 2 shapes
    gui.add(changeShapeDelay.setup("change shape delay", 15, 1, 60));
    
    // Hide controls
    bHide = false;
    
    if (roomMode == 3) {
        bHide = true;
    }
    
    /***********************
     * MARK: Allocate FBOs *
     ***********************/
    
    if (roomMode == 3 || roomMode == 4) {
        PROJECTION_WIDTH = 1920;
        PROJECTION_HEIGHT = 1200; // 1080
    } else if (roomMode == 2) {
        PROJECTION_WIDTH = 640;
        PROJECTION_HEIGHT = 480;
    } else {
        PROJECTION_WIDTH = 1280;
        PROJECTION_HEIGHT = 960;
    }

    finalFbo.allocate(PROJECTION_WIDTH,PROJECTION_HEIGHT);
    targetFbo.allocate(PROJECTION_WIDTH,PROJECTION_HEIGHT);
    
    for (int i = 0; i < NUM_SHAPE_FBOS; i++) {
        ofFbo shapeFbo;
        shapeFbo.allocate(PROJECTION_WIDTH,PROJECTION_HEIGHT);
        shapeFbos.push_back(shapeFbo);
        ofFbo textureFbo;
        textureFbo.allocate(PROJECTION_WIDTH,PROJECTION_HEIGHT);
        textureFbos.push_back(textureFbo);
    }
    
    /************************
     * MARK: Set up targets *
     ************************/
    
    // Target dimensions
    gui.add(maxInchesX.setup("maxInchesX", 94, 1, TOTAL_MAX_INCHES_X));
    gui.add(maxInchesY.setup("maxInchesY", 70, 1, TOTAL_MAX_INCHES_Y));
    
    allTargetsTouching = false;
    
    // Populate targets
    updateTargets();
    
    /*************************
     * MARK: Set up graphics *
     *************************/
    
    ofBackground(0);
    
    ofSetPolyMode(OF_POLY_WINDING_POSITIVE);
    
    ofEnableAntiAliasing();
    
    ofEnableSmoothing();
    
    /*********************
     * MARK: Set up text *
     *********************/
    
    // Font
    ofTrueTypeFont::setGlobalDpi(72);
    
    franklinBook.load("frabk.ttf", 47);//36
    franklinBook.setLineHeight(18.0f);
    franklinBook.setLetterSpacing(1.037);
    
    // Text position
    textTop = 40;
    textLeft = 1703;
    textRotate = 180;//181.62;
    textRight = 238; //687 for right justify
    textTop2 = 40;
    textRotate2 = 0;//-2.8;
    
    /**********************
     * MARK: Shape colors *
     **********************/
    
    // Shape colors
    // Gradients between neighboring colors
    
    // Red -> orange
    shapeColor redOrangeColor = {
        ofColor(219,0,67),
        ofColor(245,131,87),
        ofColor(161, 22, 18)
    };
    shapeColors.push_back(redOrangeColor);
    
    // Orange -> yellow
    shapeColor orangeYellowColor = {
        ofColor(217,43,0),
        ofColor(246,240,36),
        ofColor(208,29,23)
    };
    shapeColors.push_back(orangeYellowColor);
    
    // Yellow -> green
    shapeColor yellowGreenColor = {
        ofColor(246,240,36),
        ofColor(42,185,94),
        ofColor(4,64,50)
    };
    shapeColors.push_back(yellowGreenColor);
    
    // Green -> blue
    shapeColor greenBlueColor = {
        ofColor(42,185,94),
        ofColor(55,2,243),
        ofColor(36,0,168)
    };
    shapeColors.push_back(greenBlueColor);
    
    // Blue -> purple
    shapeColor bluePurpleColor = {
        ofColor(55,2,243),
        ofColor(187,115,239),
        ofColor(58,0,99)
    };
    shapeColors.push_back(bluePurpleColor);
    
    // Pink -> red
    shapeColor pinkRedColor = {
        ofColor(221,66,236),
        ofColor(219,0,67),
        ofColor(179,0,0)
    };
    shapeColors.push_back(pinkRedColor);
}

//--------------------------------------------------------------
void ofApp::update() {
    
    /****************
     * MARK: Kinect *
     ****************/
    
    kinect.update();
    
    // There is a new frame and we are connected
    if(kinect.isFrameNew()) {
        
        int w = kinect.getWidth();
        int h = kinect.getHeight();
        
        // Load grayscale depth image from the Kinect source
        grayImage.setFromPixels(kinect.getDepthPixels());
        
        // Threshold Kinect pixels
        ofPixels & pix = grayImage.getPixels();
        int numPixels = pix.size();
        for(int i = 0; i < numPixels; i++) {
            // Calculate row, then calculate far threshold based on row
            int row = i / kinect.width;
            int thisFarThreshold = ofMap(row, roiY, roiY+roiH, minFarThreshold, maxFarThreshold, true);
            
            // if pix is black, ignore - make it 255?
            // far threshold should be smaller than near threshold
            if(pix[i] == 00) {
                pix[i] = 255; // if black, make white to ignore visual noise
            } else if(pix[i] < nearThreshold && pix[i] > thisFarThreshold) {
                pix[i] = 255;
            } else {
                pix[i] = 0;
            }
        }
        
        // Update the CV images
        grayImage.flagImageChanged();
        
        // Blur image
        grayImage.blurGaussian(blurValue);
        
        grayImage.threshold(blurThreshold);
        
        // Set region of interest
        // Derived from measuring pixels on screen and scaling to Kinect size 640x480 (divide by 2)
        grayImage.setROI(roiX, roiY, roiW, roiH);
        
        // Find contours
        contourFinder.findContours(grayImage, minBlobArea, maxBlobArea, maxBlobNum, true); // true = find holes
        
        grayImage.resetROI();
        
        // Get pixels from FBO
        ofPixels finalFboPixels;
        finalFbo.readToPixels(finalFboPixels);
        
        ofPixels targetFboPixels;
        targetFbo.readToPixels(targetFboPixels);
        
        /**********************************
         * MARK: Detect number of players *
         **********************************/
        
        blobArea = 0;
        for (int i = 0; i < contourFinder.nBlobs; i++) {
            blobArea += contourFinder.blobs[i].area;
        }
        
        if (!twoTargets) {
            // EXPERIMENTAL two targets only
            twoTargets = true;
            updateTargets();
        }
        
        // Check every second
        // Make 2 targets if blobArea is less than 200000 (or roiW * roiH)
        if (ofGetFrameNum() % changeShapeDelay == 0) {
            // If number of players changed, update targets
            // EXPERIMENTAL - two targets only
            /*if (!twoTargets && (contourFinder.nBlobs > 1 || blobArea > onePersonBlobArea)) {
                twoTargets = true;
                updateTargets();
            } else if (twoTargets && (contourFinder.nBlobs <= 1 && blobArea <= onePersonBlobArea)) {
                twoTargets = false;
                updateTargets();
            }*/
            
            if (prevNumBlobs != contourFinder.nBlobs) {
                // Shuffle colors
                ofRandomize(shapeColors);
            }
            
            prevNumBlobs = contourFinder.nBlobs;
        }
        
        /*****************
         * MARK: Targets *
         *****************/
        
        // Iterate over every scaleVal (2 or 3) pixels, reducing evaluated area to 640 x 480, speeding up performance
        
        // Make sure target doesn't move around if no people in area
        if (contourFinder.nBlobs > 0) {
            numTrianglesTouching = 0;
            
            // Test whether all targets are touching
            for (int k = 0; k < targetTriangles.size(); k++) {
                // Temporary variable to prevent flickering
                bool touchingThisTriangle = false;
                
                // Loop through this triangle's bounding box
                for (int j = targetTriangles[k].boundingYMin; j < targetTriangles[k].boundingYMax; j += scaleVal) {
                    for (int i = targetTriangles[k].boundingXMin; i < targetTriangles[k].boundingXMax; i += scaleVal) {
                        ofColor colorAtXY = finalFboPixels.getColor(i, j);
                        ofColor targetColorAtXY = targetFboPixels.getColor(i, j);
                        
                        if (colorAtXY.getBrightness() > 0 &&
                            (targetColorAtXY.getBrightness() == k + 1 ||
                            targetColorAtXY.getBrightness() == TOUCHING_TRIANGLE_BRIGHTNESS - k ||
                            targetColorAtXY.getBrightness() == 255)) { // to account for wireframe
                            touchingThisTriangle = true;
                        }
                    }
                }
             
                targetTriangles[k].touching = touchingThisTriangle;
                
                if (touchingThisTriangle) {
                    numTrianglesTouching++;
                }
            }
        } else {
            numTrianglesTouching = 0;
        }
    }
    
    /***************************
     * MARK: Draw texture FBOs *
     ***************************/
    
    for (int i = 0; i < contourFinder.nBlobs; i++){
        ofxCvBlob thisBlob = contourFinder.blobs[i];
        
        // Color
        shapeColor thisColor = shapeColors[i % shapeColors.size()];
        ofColor lightColor = thisColor.light;
        ofColor darkColor = thisColor.dark;
        ofColor dotColor = thisColor.dot;
        
        // Begin texture fbos
        textureFbos[i].begin();
        
        ofClear(0,0,0); // background
        
        ofBackground(lightColor);
        
        ofFill();
        
        // Gradient
        glBegin(GL_QUADS);
        glColor4f((float) lightColor.r/255.,
                  (float) lightColor.g/255.,
                  (float) lightColor.b/255.,
                  1);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(PROJECTION_WIDTH, 0.0f, 0.0f);
        glColor4f((float) darkColor.r/255.,
                  (float) darkColor.g/255.,
                  (float) darkColor.b/255.,
                  1);
        glVertex3f(PROJECTION_WIDTH, PROJECTION_HEIGHT, 0.0f);
        glVertex3f(0.0f, PROJECTION_HEIGHT, 0.0f);
        glEnd();
        
        // Dots
        ofSetColor(dotColor);
        for (int x = 10; x < PROJECTION_WIDTH; x += 10) {
            for (int y = 10; y < PROJECTION_HEIGHT; y += 10) {
                float n = ofNoise(x * 0.01, y * 0.01, ofGetFrameNum() * 0.02);
                float colorN = ofNoise(x * 0.05, y * 0.05, ofGetFrameNum() * 0.1);
                ofPushMatrix();
                ofTranslate(x, y);
                ofScale(7 * n);
                
                ofColor lerpedColor = dotColor.getLerped(ofColor(255), colorN);
                ofSetColor(lerpedColor.r, lerpedColor.g, lerpedColor.b, colorN * 255.);
                
                ofDrawCircle(0, 0, 1);
                ofPopMatrix();
            }
        }
        
        ofClearAlpha();
        textureFbos[i].end();
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    
    /*****************************
     * MARK: Populate shape FBOs *
     *****************************/
    
    // Draw to final FBO
    
    finalFbo.begin();
    
    ofSetColor(255);
    ofClear(0,0,0,0);
    ofBackground(0,0,0,0);
    
    // Rotate 180
    // https://gist.github.com/KavinHan/fe4005e95a6225f6be751210836a85a2
    ofPushMatrix();
    ofTranslate(PROJECTION_WIDTH/2, PROJECTION_HEIGHT/2, 0); //move pivot to centre
    ofRotateDeg(180, 0, 0, 1); //rotate from centre
    ofPushMatrix();
    ofTranslate(-PROJECTION_WIDTH/2,-PROJECTION_HEIGHT/2,0); //move back by the centre offset
    
    /**************************
     * MARK: Draw silhouettes *
     **************************/

    for (int i = 0; i < contourFinder.nBlobs; i++){
        ofxCvBlob thisBlob = contourFinder.blobs[i];
        
        ofPolyline cur;
        cur.addVertices(thisBlob.pts);
        cur.setClosed(true);
        cur = cur.getSmoothed(smoothingSize, smoothingShape);
        
        // Copy polyline into path so it can be filled
        
        if (i < NUM_SHAPE_FBOS) {
            shapeFbos[i].begin();
            
            ofClear(0,0,0,0);
            
            // Silhouette shape
            
            ofFill();
            
            ofBeginShape();
            for( int i = 0; i < cur.getVertices().size(); i++) {
                ofVertex(cur.getVertices().at(i).x * scaleVal + shapeFboLeft,
                         cur.getVertices().at(i).y * scaleVal + shapeFboTop);
            }
            ofEndShape(true);
            
            shapeFbos[i].end();
            
            // Set texture for shape
            textureFbos[i].getTexture().setAlphaMask(shapeFbos[i].getTexture());
            textureFbos[i].draw(0,0);
        }
    }
    
    ofPopMatrix();
    ofPopMatrix();
    
    finalFbo.end();
    
    /*************************
     * MARK: Draw target FBO *
     *************************/
    
    targetFbo.begin();
    ofClear(0,0,0,0);
    ofBackground(0,0,0,0);
    
    
    // EXPERIMENT: Display bounds
    /*ofPushStyle();
    ofSetColor(0,255,0);
    ofNoFill();
    int maxPixelsX = maxInchesX * INCH_PIXELS;
    int maxPixelsY = maxInchesY * INCH_PIXELS;
    ofDrawRectangle(3 * INCH_PIXELS, 3 * INCH_PIXELS, maxPixelsX, maxPixelsY);
    ofPopStyle();*/
    
    // Draw targets
    
    allTargetsTouching = numTrianglesTouching == targetTriangles.size();
    
    /************************
     * MARK: Change targets *
     ************************/
    
    // Draw a shape at target
    if (moveTarget || (allTargetsTouching && targetLerpPercent >= 1.)) {
        // Calculate new targets
        prevTargets = nextTargets;
        targetLerpPercent = 0;
        updateTargets();
    }
    
    /***********************************
     * MARK: Draw interpolated targets *
     ***********************************/
    
    ofNoFill();
    ofSetColor(255,255,255,255);
    ofSetLineWidth(lineWidth);
    
    if (targetLerpPercent < 1.) {
        // If transition is in progress, draw new interpolated triangulation
        
        triangulation.reset();
        triangulation2.reset();
        
        // Calculate interpolated points
        
        // Add 2 more points for single shape to replace 2 middle points
        int thisNumTargets = numTargets;
        if (!twoTargets) {
            thisNumTargets += 2;
        }
        
        for (int i = 0; i < thisNumTargets; i++) {
            targetPoints[i].x = ofMap(targetLerpPercent, 0., 1., prevTargets[i].x, nextTargets[i].x, true);
            targetPoints[i].y = ofMap(targetLerpPercent, 0., 1., prevTargets[i].y, nextTargets[i].y, true);
            
            triangulation.addPoint(targetPoints[i]);
        }
        
        if (twoTargets) {
            for (int i = numTargets; i < numTargets * 2; i++) {
                targetPoints[i].x = ofMap(targetLerpPercent, 0., 1., prevTargets[i].x, nextTargets[i].x, true);
                targetPoints[i].y = ofMap(targetLerpPercent, 0., 1., prevTargets[i].y, nextTargets[i].y, true);
                
                triangulation2.addPoint(targetPoints[i]);
            }
            
            // Add 2 middle points to both triangulations to connect them
            int targetsSize = nextTargets.size();
            
            targetPoints[targetsSize - 1].x = ofMap(targetLerpPercent, 0., 1., prevTargets[targetsSize - 1].x, nextTargets[targetsSize - 1].x, true);
            targetPoints[targetsSize - 2].x = ofMap(targetLerpPercent, 0., 1., prevTargets[targetsSize - 2].x, nextTargets[targetsSize - 2].x, true);
            
            triangulation.addPoint(targetPoints[targetsSize - 1]);
            triangulation.addPoint(targetPoints[targetsSize - 2]);
            triangulation2.addPoint(targetPoints[targetsSize - 1]);
            triangulation2.addPoint(targetPoints[targetsSize - 2]);
        }
        
        targetLerpPercent += .125;
        
        triangulation.triangulate();
        triangulation.triangleMesh.drawWireframe();
        
        if (twoTargets) {
            triangulation2.triangulate();
            triangulation2.triangleMesh.drawWireframe();
        }
    } else {
        // If transition is finished, draw static triangulation
        nextTriangulation.triangleMesh.drawWireframe();
        
        if (twoTargets) {
            nextTriangulation2.triangleMesh.drawWireframe();
        }
    }
    
    /*******************************
     * MARK: Draw target triangles *
     *******************************/
    
    ofFill();
    
    // Loop over triangles to draw them
    if (targetLerpPercent >= 1.) {
        for (int i = 0; i < targetTriangles.size(); i++) {
            
            if (targetTriangles[i].touching) {
                ofSetColor(255,255,255,TOUCHING_TRIANGLE_BRIGHTNESS - i);
            } else {
                ofSetColor(255,255,255,i + 1);
            }
            
            ofDrawTriangle(targetTriangles[i].point1, targetTriangles[i].point2, targetTriangles[i].point3);
        }
    }
    
    targetFbo.end();
    
    /**************************
     * MARK: Draw from Kinect *
     **************************/
    
    ofPushMatrix();
    
    ofSetColor(255,255,255,255);

    // Full screen
    ofTranslate(220,0); // accommodate UI menu

    int drawVideoX = 0;
    int drawVideoY = 0;
    int drawVideoWidth = 640;
    int drawVideoHeight = 480;

    // Kinect video on first screen
    if (videoMode == 1) {
        // Kinect video image
        kinect.draw(drawVideoX, drawVideoY, drawVideoWidth, drawVideoHeight);
    } else if (videoMode == 2) {
        // Depth image
        kinect.drawDepth(drawVideoX, drawVideoY, drawVideoWidth, drawVideoHeight);
    } else if (videoMode == 3) {
        // Threshold image
        grayImage.draw(drawVideoX, drawVideoY, drawVideoWidth, drawVideoHeight);
    } else if (videoMode == 4) {
        // Contours
        ofPushMatrix();
        ofTranslate(roiX, roiY);
        contourFinder.draw(drawVideoX, drawVideoY, drawVideoWidth, drawVideoHeight);
        ofPopMatrix();
    }

    // Draw ROI on 1st screen
    if (roomMode != 3 && videoMode != 0) {
        ofPushStyle();
        ofNoFill();
        ofSetColor(0,255,0);
        ofDrawRectangle(roiX, roiY, roiW, roiH);
        ofPopStyle();
    }
    
    ofPopMatrix();
    
    /*************************
     * MARK: Draw final FBOs *
     *************************/

    // Draw final FBO on second screen (projection)
    
    ofSetColor(255);
    
    ofPushMatrix();
    
    if (roomMode == 2 || roomMode == 3) {
        // Draw shapes
        finalFbo.draw(fboLeft, fboTop);
        
        // Draw targets
        targetFbo.draw(fboLeft, fboTop);
    } else {
        // Draw shapes
        finalFbo.draw(SCREEN_WIDTH + fboLeft, fboTop);
        
        // Draw targets
        targetFbo.draw(SCREEN_WIDTH + fboLeft, fboTop);
    }
    
    ofPopMatrix();
    
    /*******************
     * MARK: Draw text *
     *******************/
    
    ofPushMatrix();
    
    if (roomMode == 3) {
        ofTranslate(textLeft, textTop);
    } else {
         ofTranslate(SCREEN_WIDTH + textLeft, textTop);
    }
    
    ofRotateDeg(textRotate);
    ofSetColor(255);
    //franklinBook.drawString("LIGHT TOUCHES SKIN", 0, -1 * SCREEN_HEIGHT - 150);
    ofRotateDeg(textRotate2);
    franklinBook.drawString("MOVE YOUR BODIES TO TOUCH ALL THE TRIANGLES", textRight, -1 * SCREEN_HEIGHT - 150); //textTop2
    
    ofPopMatrix();
    
    /*************************
     * MARK: Draw debug text *
     *************************/
    
    if (roomMode != 3 && !bHide) {
        ofPushStyle();
        
        stringstream reportStream;
        
        reportStream << "Target triangles touching: " << numTrianglesTouching << "/" << targetTriangles.size() << endl;
        
        reportStream << "Num blobs: " << contourFinder.nBlobs << endl;

        reportStream << "Blob area: " << blobArea << endl;

        reportStream << "Target lerp percent: " << targetLerpPercent << endl;
        
        ofDrawBitmapString(reportStream.str(), 220, 550);
        
        // Attempt to draw blob area... can't draw numbers
        //franklinBook.drawString("Blob Area:", 220, 750);
        
        ofPopStyle();
    }
    
    // Draw GUI
    if(!bHide){
        gui.draw();
    }
}

//--------------------------------------------------------------
void ofApp::exit() {
    kinect.close();
}

//--------------------------------------------------------------
void ofApp::keyPressed (int key) {
    switch (key) {
            
        case 'o':
            kinect.open();
            break;
            
        case 'c':
            kinect.close();
            break;
            
        case '0':
            // No video
            videoMode = 0;
            break;
            
        case '1':
            // Kinect video
            videoMode = 1;
            break;
            
        case '2':
            // Depth image
            videoMode = 2;
            break;
            
        case '3':
            // Threshold image
            videoMode = 3;
            break;
            
        case '4':
            // Contours
            videoMode = 4;
            break;
            
        case 'h':
            // Press h to toggle show/hide GUI
            bHide = !bHide;
            break;
            
        case ' ':
            // Press space to manually move target
            moveTarget = true;
            break;
    }
}

//--------------------------------------------------------------
// Custom function that takes an index and returns the coordinates of the triangle we refer to
vector <ofPoint> ofApp::getTriangle(int i, ofxDelaunay thisTriangulation){
    int pA = thisTriangulation.triangleMesh.getIndex(i*3);
    int pB = thisTriangulation.triangleMesh.getIndex(i*3+1);
    int pC = thisTriangulation.triangleMesh.getIndex(i*3+2);
    
    ofPoint pointA = thisTriangulation.triangleMesh.getVertex(pA);
    ofPoint pointB = thisTriangulation.triangleMesh.getVertex(pB);
    ofPoint pointC = thisTriangulation.triangleMesh.getVertex(pC);
    
    vector <ofPoint> points;
    points.push_back(pointA);
    points.push_back(pointB);
    points.push_back(pointC);
    return points;
}

//--------------------------------------------------------------
void ofApp::updateTargets() {
    // Maximum dimensions in pixels
    int maxPixelsX = maxInchesX * INCH_PIXELS;
    int maxPixelsY = maxInchesY * INCH_PIXELS;
    
    // Target margins
    int topMargin = 3 * INCH_PIXELS;
    int leftMargin = 3 * INCH_PIXELS;
    
    if (roomMode == 2 && roiW) {
        maxPixelsX = roiW - leftMargin;
        maxPixelsY = roiH - topMargin;
    }
    
    // Calculate random position based on max pixels margins
    
    // Left margin for single shape
    int targetPositionX = 0;
    
    // Get random tile shapes
    tileShapeNum = ofRandom(NUM_TILE_SHAPES);
    tileShapeNum2 = ofRandom(NUM_TILE_SHAPES);
    
    // Dividing axis between two triangulated shapes
    dividingAxis = ofRandom(maxPixelsX * .25, maxPixelsX * .75);
    
    // Size per tile in shape
    blockSize = dividingAxis/3;
    
    // If single target, calculate left position
    if (!twoTargets) {
        targetPositionX = ofRandom(leftMargin, TOTAL_MAX_INCHES_X * INCH_PIXELS - dividingAxis);
        
        if (roomMode == 2) {
            targetPositionX = 0;
        }
    }
    
    // Is the generated point in the tile shape?
    bool pointInTileShape = false;
    
    // Number of target points in triangulation
    int thisNumTargets = numTargets;
    
    // Triangles
    nextTriangulation.reset();
    nextTriangulation2.reset();
    nextTargets.clear();
    
    triangulation.reset();
    triangulation2.reset();
    targetPoints.clear();
    
    targetTriangles.clear();
    
    // Add 2 more points for single shape to replace 2 middle points
    if (!twoTargets) {
        thisNumTargets += 2;
    }
    
    // Populate next triangulation
    
    // Left triangulation
    
    while (nextTriangulation.getNumPoints() < thisNumTargets) {
        int thisPointX = ofRandom(leftMargin, dividingAxis - minTriangleUnit);
        int thisPointY = ofRandom(topMargin, maxPixelsY);

        // Iterate over each square in tile shape - is this point within it?
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (tileShapes[tileShapeNum][i][j] == 1 &&
                    thisPointX > i * blockSize &&
                    thisPointX < (i+1) * blockSize &&
                    thisPointY > j * blockSize &&
                    thisPointY < (j+1) * blockSize) {
                    pointInTileShape = true;
                }
            }
        }
        
        if (pointInTileShape) {
            nextTriangulation.addPoint(ofPoint(targetPositionX + thisPointX, thisPointY));
            nextTargets.push_back(ofPoint(targetPositionX + thisPointX, thisPointY));
        }
    }
    
    // Right triangulation
    
    if (twoTargets) {
        blockSize = (maxPixelsX - dividingAxis)/3;
        
        while (nextTriangulation2.getNumPoints() < numTargets) {
            int thisPointX = ofRandom(leftMargin, maxPixelsX - dividingAxis + minTriangleUnit);
            int thisPointY = ofRandom(topMargin, maxPixelsY);
            
            // Iterate over each square in tile shape - is this point within it?
            pointInTileShape = false;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (tileShapes[tileShapeNum2][i][j] == 1 &&
                        thisPointX > i * blockSize &&
                        thisPointX < (i+1) * blockSize &&
                        thisPointY > j * blockSize &&
                        thisPointY < (j+1) * blockSize) {
                        pointInTileShape = true;
                    }
                }
            }
            
            if (pointInTileShape) {
                nextTriangulation2.addPoint(ofPoint(dividingAxis + thisPointX, thisPointY));
                nextTargets.push_back(ofPoint(dividingAxis + thisPointX, thisPointY));
            }
        }
        
        // Add points in middle to connect two triangulated shapes
        
        int randomYTop = ofRandom(topMargin, maxPixelsY);
        
        nextTriangulation.addPoint(ofPoint(dividingAxis, randomYTop));
        nextTriangulation2.addPoint(ofPoint(dividingAxis, randomYTop));
        
        nextTargets.push_back(ofPoint(dividingAxis, randomYTop));
        
        int randomYBottom = ofRandom(randomYTop + minDividingHeight, maxPixelsY);
        
        nextTriangulation.addPoint(ofPoint(dividingAxis, randomYBottom));
        nextTriangulation2.addPoint(ofPoint(dividingAxis, randomYBottom));
        
        nextTargets.push_back(ofPoint(dividingAxis, randomYBottom));
        
        nextTriangulation2.triangulate();
    }
    
    nextTriangulation.triangulate();
    
    // Initialize target triangles the first time drawing left shape
    if (firstTime) {
        prevTargets = nextTargets;
        targetPoints = nextTargets;
        
        triangulation = nextTriangulation;
        
        firstTime = false;
    }
    
    // Initialize target triangles the first time drawing right shape
    if (twoTargets && firstTime2ndShape) {
        prevTargets = nextTargets;
        targetPoints = nextTargets;
        
        triangulation = nextTriangulation;
        triangulation2 = nextTriangulation2;
        
        firstTime2ndShape = false;
    }
    
    // Loop over the triangles in left shape
    for (int g = 0; g < nextTriangulation.getNumTriangles(); g++){
        vector <ofPoint> pts = getTriangle(g, nextTriangulation); // extract the vector with 3 points
        
        // Get bounding rect for each triangle
        int thisBoundingXMin = PROJECTION_WIDTH;
        int thisBoundingYMin = PROJECTION_HEIGHT;
        int thisBoundingXMax = 0;
        int thisBoundingYMax = 0;
        
        for (int i = 0; i < 3; i++) {
            if (pts[i].x < thisBoundingXMin) {
                thisBoundingXMin = pts[i].x;
            }
            if (pts[i].y < thisBoundingYMin) {
                thisBoundingYMin = pts[i].y;
            }
            if (pts[i].x > thisBoundingXMax) {
                thisBoundingXMax = pts[i].x;
            }
            if (pts[i].y > thisBoundingYMax) {
                thisBoundingYMax = pts[i].y;
            }
        }
        
        // Populate vector of triangles
        targetTriangle myTriangle = {
            pts[0],
            pts[1],
            pts[2],
            false,
            thisBoundingXMin,
            thisBoundingYMin,
            thisBoundingXMax,
            thisBoundingYMax
        };
        
        targetTriangles.push_back(myTriangle);
    }
    
    // Loop over the triangles in right shape
    for (int g = 0; g < nextTriangulation2.getNumTriangles(); g++){
        vector <ofPoint> pts = getTriangle(g, nextTriangulation2); // extract the vector with 3 points
        
        // Get bounding rect for each triangle
        int thisBoundingXMin = PROJECTION_WIDTH;
        int thisBoundingYMin = PROJECTION_HEIGHT;
        int thisBoundingXMax = 0;
        int thisBoundingYMax = 0;
        
        for (int i = 0; i < 3; i++) {
            if (pts[i].x < thisBoundingXMin) {
                thisBoundingXMin = pts[i].x;
            }
            if (pts[i].y < thisBoundingYMin) {
                thisBoundingYMin = pts[i].y;
            }
            if (pts[i].x > thisBoundingXMax) {
                thisBoundingXMax = pts[i].x;
            }
            if (pts[i].y > thisBoundingYMax) {
                thisBoundingYMax = pts[i].y;
            }
        }
        
        // Populate vector of triangles
        targetTriangle myTriangle = {
            pts[0],
            pts[1],
            pts[2],
            false,
            thisBoundingXMin,
            thisBoundingYMin,
            thisBoundingXMax,
            thisBoundingYMax
        };
        
        targetTriangles.push_back(myTriangle);
    }
    
    moveTarget = false;
}
