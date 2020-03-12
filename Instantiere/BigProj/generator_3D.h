// Tema3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
using namespace std;
#define _USE_MATH_DEFINES
#include <math.h>
#include <fstream>
#include <cstdlib>
#include "glm/glm/glm.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm/gtx/transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
using namespace glm;
void generate_circle()
{
	int k, n = 51;
	long double R = 1000, tau = 2 * M_PI;
	ofstream out("circle.txt");
	out << n << '\n';
	out << 0.0 << ' ' << 0.0 << ' ' << 0.0 << ' ' << 1.0 << '\n';
	for (k = 0; k <= n; k++)
		out << (float)R * cos(k*tau / n) << ' ' << (float)R * sin(k*tau / n) << ' ' << 0.0 << ' ' << 1.0 << '\n';
	out << 1.0 << ' ' << 1.0 << ' ' << 1.0 << ' ' << 1.0 << '\n';
	out << 0.0 << ' ' << 0.0 << ' ' << 0.0 << ' ' << 1.0 << '\n';
	for (k = 1; k < n; k++)
		out << 0.1 << ' ' << 0.5 << ' ' << 1.0 << ' ' << 1.0 << '\n';
	out << 0.0 << ' ' << 0.0 << ' ' << 0.0 << ' ' << 1.0 << '\n';

	out.close();
}
void generate_sphere(int n)
{
	int k, j;
	long double R = 2, tau = 2 * M_PI;
	ofstream out("sphere.obj");
	//out << n << '\n';
	for (k = 0; k <= n; k++)
		for (j = 0; j <= n; j++)
			out <<"v "<< (float)R * cos((k - (float)n / 2)*M_PI / n) * cos(j*tau / n) << ' '
			<< (float)R * cos((k - (float)n / 2)*M_PI / n) * sin(j*tau / n) << ' '
			<< (float)R * sin((k - (float)n / 2)*M_PI / n) << '\n';
	/*for (k = 0; k <= n; k++)
		for (j = 0; j <= n; j++)
			out << abs((float)cos((k - (float)n / 2)*M_PI / n) * cos(j*tau / n)) << ' '
			<< abs((float)cos((k - (float)n / 2)*M_PI / n) * sin(j*tau / n)) << ' '
			<< abs((float)sin((k - (float)n / 2)*M_PI / n)) << ' ' << 1.0 << '\n';*/
	vec3 normal;
	for (k = 0; k <= n; k++)
		for (j = 0; j <= n; j++)
		{
			normal = vec3((float)R * cos((k - (float)n / 2)*M_PI / n) * cos(j*tau / n),
				(float)R * cos((k - (float)n / 2)*M_PI / n) * sin(j*tau / n),
				(float)R * sin((k - (float)n / 2)*M_PI / n));
			normal = normalize(normal);
			out << "vn " << normal.x << ' ' << normal.y << ' ' << normal.z << '\n';
		}
	//out << 2 * 3 * n*n << '\n';
	int p1, p2, p3, p4;
	for (k = 0; k < n; k++)
		for (j = 0; j < n; j++)
		{
			p1 = k * (n + 1) + j +(1);
			p2 = k * (n + 1) + j + 1 + (1);
			p3 = (k + 1) * (n + 1) + j + 1 + (1);
			p4 = (k + 1) * (n + 1) + j + (1);
			out << "f " << p1 << "//" << p1 << ' ' << p2 << "//" << p2 << ' ' << p3 << "//" << p3 << '\n'
				<< "f " << p4 << "//" << p4 << ' ' << p1 << "//" << p1 << ' ' << p3 << "//" << p3 << '\n';
		}
	out.close();
}

void generate_hyperboloid(int n)
{
	int k, j;
	long double R = 2, tau = 2 * M_PI;
	ofstream out("hyperboloid.obj");
	//out << n << '\n';
	for (k = 0; k <= n; k++)
		for (j = 0; j <= n; j++)
		{
			float t = ((float)k / n) * 6 - 3;
			float x = 1;
			float y = t;
			float z = t;
			float r = sqrt(x*x + y * y);
			out <<"v "<< (float)R * r * cos(j*tau / n) << ' '
				<< (float)R * r * sin(j*tau / n) << ' '
				<< (float)R * z << '\n';
		}
	/*for (k = 0; k <= n; k++)
		for (j = 0; j <= n; j++)
			out << abs((float)cos((k - (float)n / 2)*M_PI / n) * cos(j*tau / n)) << ' '
			<< abs((float)cos((k - (float)n / 2)*M_PI / n) * sin(j*tau / n)) << ' '
			<< abs((float)sin((k - (float)n / 2)*M_PI / n)) << ' ' << 1.0 << '\n';*/
	vec3 normal, vx, vy;
	for (k = 0; k <= n; k++)
		for (j = 0; j <= n; j++)
		{
			float t = ((float)k / n) * 6 - 3;
			float x = 1;
			float y = t;
			float z = t;
			float r = sqrt(x*x + y * y);
			vx = vec3((float)R / r * t*cos(j*tau / n), (float)R / r * t*sin(j*tau / n), (float)R);
			vy = vec3(-(float)R * r * sin(j*tau / n), (float)R * r * cos(j*tau / n), 0.0);
			normal = normalize(cross(vx,vy));
			//normal = vec3((float)R * r * cos(j*tau / n),
				//(float)R * r * sin(j*tau / n),
				//-(float)R * z);
			//normal = normalize(normal);
			out << "vn " << normal.x << ' ' << normal.y << ' ' << normal.z << '\n';
		}
	int p1, p2, p3, p4;
	//out << 2 * 3 * n*n << '\n';
	for (k = 0; k < n; k++)
		for (j = 0; j < n; j++)
		{
			p1 = k * (n + 1) + j +1;
			p2 = k * (n + 1) + j + 1+1;
			p3 = (k + 1) * (n + 1) + j + 1+1;
			p4 = (k + 1) * (n + 1) + j+1;
			out << "f " << p1 << "//" << p1 << ' ' << p2 << "//" << p2 << ' ' << p3 << "//" << p3 << '\n'
				<< "f " << p4 << "//" << p4 << ' ' << p1 << "//" << p1 << ' ' << p3 << "//" << p3 << '\n';

		}out.close();
}
void generate_hyperboloid_2_sheet(int n)
{
	int k, j;
	long double R = 1, tau = 2 * M_PI;
	ofstream out("hyperboloid_2_sheet.obj");
	//out << n << '\n';
	for (k = 0; k <= n; k++)
		for (j = 0; j <= n; j++)
		{
			float t = ((float)k / n) * 4;
			float x = sinh(t);
			float dx = cosh(t);
			float y = 0;
			float dy = 0;
			float z = cosh(t);
			float dz = sinh(t);
			float r = sqrt(x*x + y * y);
			out << "v " << (float)R * r * cos(j*tau / n) << ' '
				<< (float)R * r * sin(j*tau / n) << ' '
				<< (float)R * z << '\n';
		}
	vec3 normal, vx, vy;
	for (k = 0; k <= n; k++)
		for (j = 0; j <= n; j++)
		{
			float t = ((float)k / n) * 4;
			float x = sinh(t);
			float dx = cosh(t);
			float y = 0;
			float dy = 0;
			float z = cosh(t);
			float dz = sinh(t);
			float r = sqrt(x*x + y * y);
			if (r > 0.0001)
			{
				vx = vec3((float)R / r * (x*dx + y * dy)*cos(j*tau / n), (float)R / r * (x*dx + y * dy)*sin(j*tau / n), (float)R*dz);
				vy = vec3(-(float)R * r * sin(j*tau / n), (float)R * r * cos(j*tau / n), 0.0);
				normal = normalize(cross(vy, vx));
			}
			else normal = normalize(vec3(0.0f, 1.0f, 0.0f));
			//normal = vec3((float)R * r * cos(j*tau / n),
				//(float)R * r * sin(j*tau / n),
				//-(float)R * z);
			//normal = normalize(normal);
			out << "vn " << normal.x << ' ' << normal.y << ' ' << normal.z << '\n';
		}
	int p1, p2, p3, p4;
	//out << 2 * 3 * n*n << '\n';
	for (k = 0; k < n; k++)
		for (j = 0; j < n; j++)
		{
			p1 = k * (n + 1) + j + 1;
			p2 = k * (n + 1) + j + 1 + 1;
			p3 = (k + 1) * (n + 1) + j + 1 + 1;
			p4 = (k + 1) * (n + 1) + j + 1;
			out << "f " << p1 << "//" << p1 << ' ' << p2 << "//" << p2 << ' ' << p3 << "//" << p3 << '\n'
				<< "f " << p4 << "//" << p4 << ' ' << p1 << "//" << p1 << ' ' << p3 << "//" << p3 << '\n';

		}out.close();
}