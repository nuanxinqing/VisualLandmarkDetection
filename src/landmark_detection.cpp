/*******************************************************************************

INTEL CORPORATION PROPRIETARY INFORMATION
This software is supplied under the terms of a license agreement or nondisclosure
agreement with Intel Corporation and may not be copied or disclosed except in
accordance with the terms of that agreement
Copyright(c) 2012 Intel Corporation. All Rights Reserved.

*******************************************************************************/
#include "util_pipeline.h"
#include "face_render.h"
#include <stdlib.h>
#include <windows.h>
#include <stdio.h>

#pragma comment( lib, "user32.lib" )

class FacePipeline: public UtilPipeline {
public:
    FacePipeline(void):UtilPipeline() {
        m_face_render = NULL; 
		
        m_face_render = new  FaceRender(L"Utilee");
      
		EnableFaceLandmark();
    }

	virtual ~FacePipeline() {
		if (m_face_render != NULL){
            m_face_render->Release();
            m_face_render = NULL;
        }
	}


    virtual bool OnNewFrame(void) {

        /* face */
        PXCFaceAnalysis *faceAnalyzer = QueryFace();
		

		PXCFaceAnalysis::Landmark *landmark = faceAnalyzer->DynamicCast<PXCFaceAnalysis::Landmark>();
		
	
	    // loop all faces
        m_face_render->ClearData();
        for (int fidx = 0; ; fidx++) 
		{
            pxcUID fid = 0;
            pxcU64 timeStamp = 0;
            pxcStatus sts = faceAnalyzer->QueryFace(fidx, &fid, &timeStamp);
            			
			if (sts < PXC_STATUS_NO_ERROR) break; // no more faces
            
			m_face_render->SetLandmarkData (landmark, fid);
			m_face_render->PrintLandmarkData(landmark, fid);
			
//		system("shutdown -i -h");  
		
			
			/*
			if( !LockWorkStation() )
        printf ("LockWorkStation failed with %d\n", GetLastError());*/
		
		
		}
		
		return(m_face_render->RenderFrame( QueryImage(PXCImage::IMAGE_TYPE_COLOR)) );
		
	}

protected:
	FaceRender*         m_face_render;
};



static FacePipeline pipeline;
int wmain(int argc, WCHAR* argv[]) {

	
	
	if (!pipeline.LoopFrames()) wprintf_s(L"Failed to intialize or stream data\n"); 
    return 0;
}