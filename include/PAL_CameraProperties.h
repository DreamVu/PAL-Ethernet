# ifndef PAL_CAMERA_PROPERTIES_H
# define PAL_CAMERA_PROPERTIES_H

namespace PAL
{

	enum CameraPropertyFlags
	{
	    BRIGHTNESS = 0x1,
		CONTRAST = 0x2,
		SATURATION = 0x4,
		GAMMA = 0x8,
		GAIN = 0x10,
		WHITE_BAL_TEMP = 0x20,
		SHARPNESS = 0x40,
		EXPOSURE = 0x80,
		AUTO_WHITE_BAL = 0x100,
		AUTO_EXPOSURE = 0x200,
		RESOLUTION = 0x400,
		VERTICAL_FLIP = 0x800,
		FILTER_DISPARITY = 0x1000,
		FILTER_SPOTS = 0x2000,
		PROJECTION = 0x4000,
		DISPARITY_COMPUTATION = 0x8000,
		CAMERA_HEIGHT = 0x10000,
		FOV = 0x20000,
		ALL = 0x3FFFF,
	};

	struct Resolution
	{
		int width;
		int height;
	};

	enum Acknowledgement
	{
		IGNORED, 
		SUCCESS, 
		FAILURE, 
		INVALID_PROPERTY_VALUE, 
		ERROR_CAMERA_NOT_INITIALIZED
	};

	enum Projection
	{
		EQUI_RECTANGULAR = 0,
		PERSPECTIVE = 1,
	};
	
	enum DisparityComputation
	{
		HIGH_QUALITY_A = 0,
		HIGH_QUALITY_B = 1,
	};
	
       enum FilterSpots
	{
		DISABLED = 0,
	    FAST_FS = 1,
	    HIGH_QUALITY_FS = 2,	
	};


	struct CameraProperties
	{
	    int brightness;
	    int contrast;
		int saturation;
		int gamma;
		int gain;
		int white_bal_temp;
		int sharpness;
		int exposure;
		bool  auto_white_bal;
		bool  auto_exposure;

		Resolution resolution;
			
		bool  vertical_flip;
		bool  filter_disparity;
		FilterSpots  filter_spots;
        
		//Projection type : equi-rectangular or perspective
		Projection projection;
		
		//Should this be fast but of lower quality?
		//or of high quality but slow
		DisparityComputation computation; 

		int camera_height;

		int fov_start;
		int fov_end; 
		     
        static const int MAX_BRIGHTNESS = 15;
		static const int MIN_BRIGHTNESS = -15;
		static const int DEFAULT_BRIGHTNESS = 0;
		
		static const int MAX_CONTRAST = 30;
		static const int MIN_CONTRAST = 0;
		static const int DEFAULT_CONTRAST = 4;
		
		static const int MAX_SATURATION = 60;
		static const int MIN_SATURATION = 0;
		static const int DEFAULT_SATURATION = 30;

		static const int MAX_GAMMA = 500;
		static const int MIN_GAMMA = 40;
		static const int DEFAULT_GAMMA = 220;

		static const int MAX_GAIN = 100;
		static const int MIN_GAIN = 1;
		static const int DEFAULT_GAIN = 4;

		static const int MAX_WHITE_BAL_TEMP = 10000;
		static const int MIN_WHITE_BAL_TEMP = 10;
		static const int DEFAULT_WHITE_BAL_TEMP = 4500;
		
		static const int MAX_SHARPNESS = 127;
		static const int MIN_SHARPNESS = 0;
		static const int DEFAULT_SHARPNESS = 0;
		
		static const int MAX_EXPOSURE = 10000;
		static const int MIN_EXPOSURE = 1;
		static const int DEFAULT_EXPOSURE = 500;
				
		static const bool DEFAULT_AUTO_WHITE_BAL = 1;
		static const bool DEFAULT_AUTO_EXPOSURE = 0;

		static const Resolution DEFAULT_RESOLUTION;

		static const bool DEFAULT_VERTICAL_FLIP = false;
		static const bool DEFAULT_FILTER_DISPARITY = true;
		static const FilterSpots DEFAULT_FILTER_SPOTS = FAST_FS;
        
		static const Projection DEFAULT_PROJECTION = PERSPECTIVE;
		static const DisparityComputation DEFAULT_COMPUTATION = HIGH_QUALITY_A;

		static const int DEFAULT_CAMERA_HEIGHT = 100;

		static const int DEFAULT_FOV_START = 0;
		static const int DEFAULT_FOV_END = 360;

		CameraProperties() :
			brightness			(DEFAULT_BRIGHTNESS),
			contrast			(DEFAULT_CONTRAST), 
			saturation			(DEFAULT_SATURATION),
			gamma				(DEFAULT_GAMMA),
			gain				(DEFAULT_GAIN),
			white_bal_temp		(DEFAULT_WHITE_BAL_TEMP),
			sharpness			(DEFAULT_SHARPNESS),
			exposure			(DEFAULT_EXPOSURE)	,
			auto_white_bal		(DEFAULT_AUTO_WHITE_BAL),
			auto_exposure		(DEFAULT_AUTO_EXPOSURE),
			resolution			(DEFAULT_RESOLUTION),
			vertical_flip		(DEFAULT_VERTICAL_FLIP),
			filter_disparity	(DEFAULT_FILTER_DISPARITY),
			filter_spots		(DEFAULT_FILTER_SPOTS),
			projection			(DEFAULT_PROJECTION),
			computation			(DEFAULT_COMPUTATION),
			camera_height		(DEFAULT_CAMERA_HEIGHT),
			fov_start			(DEFAULT_FOV_START),
			fov_end				(DEFAULT_FOV_END)			
		{
		}
	};

}//namespace

# endif //PAL_CAMERA_PROPERTIES_H

