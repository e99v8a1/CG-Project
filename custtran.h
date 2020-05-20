//custom header file for all transformations

#define PI 3.14159

//custom function to rotate along an axis parallel to Y axis
void rotate(float ang)
{
	ang = (PI/180) * ang;

	float rot[16] = { 
						cos(ang), 0, sin(ang), 0,
						0, 1, 0, 0,
						-sin(ang), 0, cos(ang), 0,
						0, 0, 0, 1  
				 	};

	glMultMatrixf(rot);			   
}

//custom translation function for movement in X-Z plane
void translate(float x, float z)
{
	float trans[16] = {
						1, 0, 0, 0,
						0, 1, 0, 0, 
						0, 0, 1, 0,
						x, 0, z, 1
					 };

	glMultMatrixf(trans);				 
}

//custom shear function
void shear(float shx, float shy)
{
	float sh[] = { 
   					1, shy, 0, 0, 
   					shx, 1, 0, 0,
    				0, 0, 1, 0,
    				0, 0, 0, 1 
    			};

	glMultMatrixf(sh);
}