/* 
NTU CSIE ICG2012
R01944040 Liu Chengyin
*/

#include "TRIModel.h"
#include "utils.h"
#include <cstring>
#include <cmath>
#include <cstdlib>

Vector &Vector::operator+= (const Vector &v) {
	x += v.x; 
	y += v.y; 
	z += v.z;
	return *this;
}

Vector &Vector::operator-= (const Vector &v) {
	x -= v.x; 
	y -= v.y; 
	z -= v.z;
	return *this;
}

Matrix4x4::Matrix4x4(float _m[16]) 
{
	memcpy(m, _m, sizeof(float) * 16);
}

void Matrix4x4::setIdentity() 
{
	m[0+4*0] = 1; m[0+4*1] = 0; m[0+4*2] = 0; m[0+4*3] = 0;
    m[1+4*0] = 0; m[1+4*1] = 1; m[1+4*2] = 0; m[1+4*3] = 0;
    m[2+4*0] = 0; m[2+4*1] = 0; m[2+4*2] = 1; m[2+4*3] = 0;
    m[3+4*0] = 0; m[3+4*1] = 0; m[3+4*2] = 0; m[3+4*3] = 1;
}

Matrix4x4 &Matrix4x4::invert() const
{
	float t;
    int i, j, k, swap;
    float tmp[4][4];

	Matrix4x4 inverse;
    inverse.setIdentity();
    
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            tmp[i][j] = m[i*4+j];
        }
    }
    
    for (i = 0; i < 4; i++) {
        /* look for largest element in column. */
        swap = i;
        for (j = i + 1; j < 4; j++) {
            if (fabs(tmp[j][i]) > fabs(tmp[i][i])) {
                swap = j;
            }
        }
        
        if (swap != i) {
            /* swap rows. */
            for (k = 0; k < 4; k++) {
                t = tmp[i][k];
                tmp[i][k] = tmp[swap][k];
                tmp[swap][k] = t;
                
                t = inverse[i*4+k];
                inverse[i*4+k] = inverse[swap*4+k];
                inverse[swap*4+k] = t;
            }
        }
        
        t = tmp[i][i];
        for (k = 0; k < 4; k++) {
            tmp[i][k] /= t;
            inverse[i*4+k] /= t;
        }
        for (j = 0; j < 4; j++) {
            if (j != i) {
                t = tmp[j][i];
                for (k = 0; k < 4; k++) {
                    tmp[j][k] -= tmp[i][k]*t;
                    inverse[j*4+k] -= inverse[i*4+k]*t;
                }
            }
        }
    }

	return inverse;
}

Vector Matrix4x4::mul(const Vector &p) const  
{
	/*
		[ 0  4  8 12] [x] [x']
		[ 1  5  9 13]*[y]=[y']
		[ 2  6 10 14] [z] [z']
		[ 3  7 11 15] [1] [w ]
	*/
	Vector result;
	result.x = m[0] * p.x + m[4] * p.y +  m[8] * p.z + m[12];
	result.y = m[1] * p.x + m[5] * p.y +  m[9] * p.z + m[13];
	result.z = m[2] * p.x + m[6] * p.y + m[10] * p.z + m[14];
	float  w = m[3] * p.x + m[7] * p.y + m[11] * p.z + m[15];
	result.x /= w;
	result.y /= w;
	result.z /= w;
	return result;
}

Matrix4x4 Matrix4x4::mul(const Matrix4x4 &n) const  
{
	Matrix4x4 result;
	for(int i = 0; i < 4; i++) 
		for(int j = 0; j < 4; j++) 
			result[i + j * 4] = 0.0;
	for(int j = 0; j < 4; j++) 
		for(int k = 0; k < 4; k++)
			for(int i = 0; i < 4; i++) 
				result[i + j * 4] += m[i + k * 4] * n[k + j * 4];
	return result;
}


