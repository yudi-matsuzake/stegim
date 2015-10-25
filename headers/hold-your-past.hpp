/**********************************************************
 * HOLD YOUR PAST 
 * ********************************************************
 * Essa classe é responsável por todas as operações e
 * rotinas em em um VideoCapture para transformar um
 * quadrilátero verde nos frames anteriores recursivamente.
 *********************************************************/
#ifndef _HOLD_YOUR_PAST_HPP_
#define _HOLD_YOUR_PAST_HPP_

//--INCLUDE AND NAMESPACE----------------------------------
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "debug.hpp"

using namespace cv;


//--ENUMS ÚTEIS--------------------------------------------
typedef enum{ H, L, S } HLS_COLOR;
typedef enum{ B, G, R } BGR_COLOR;

//--FUNÇÕES AUXILIARES-------------------------------------

//--HoldYourPast class-------------------------------------
class HoldYourPast {
private:
	VideoCapture vcap; //Webcam ou vídeo onde será aplicado o hold-your-past
	
	//buffers
	Mat previous_frame;
	Mat frame_buffer_hls;
	Mat mask;
	Mat mask_blur;
	Mat debug_img;

	//pipeline function
	bool pipeline(Mat& frame_buffer);
public:
	void inicialize();			/*Constrói coisas que é igual tanto
						pra webcam quanto pro video*/

	HoldYourPast(int webcam);		/*Constrói hold-your-past com o
						  número de uma webcam*/

	HoldYourPast(string video_path);	//Constrói com um arquivo de vídeo
	~HoldYourPast();			//Destrói

	bool next(Mat& frame_buffer);		//Coloca o próximo frame da aplicação no frame_buffer

	Mat& get_debug_img();			//retorna a imagem de debug
	bool is_debug_on();			//retorna se o debug está ligado
};

#endif //_HOLD_YOUR_PAST_HPP_
