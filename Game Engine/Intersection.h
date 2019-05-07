#pragma once

#include <limits>

#include "Ray.h"

// remove this when we can import
struct bBox
{
	glm::vec3 vMin; // smallest possible vector
	glm::vec3 vMax; // largest possible vector
	glm::vec3 center; // center point of the bBox
};
//--------------------------------

class Intersection
{
public:
	Intersection() = delete;
	Intersection(const Intersection&) = delete;
	Intersection& operator=(const Intersection&) = delete;

	static bool rayBoxTest(const RayData rayData, const bBox& bBox, const glm::mat4 modelMatrix)
	{
		//glm::mat4 modelMatrix = glm::mat4(1); // get this from gameObject[i]?

		float tMin = -1000.f;
		float tMax = 1000.f;

		glm::vec3 p = bBox.center - rayData.rayOrigin;

		glm::vec3 OBBposition_worldspace(modelMatrix[3].x, modelMatrix[3].y, modelMatrix[3].z);
		glm::vec3 delta = OBBposition_worldspace - rayData.rayOrigin;
	
		// Test intersection with the 2 planes perpendicular to the OBB's X axis
		glm::vec3 xAxis(modelMatrix[0].x, modelMatrix[0].y, modelMatrix[0].z);
		float e = glm::dot(xAxis, delta);
		float f = glm::dot(rayData.rayDirection, xAxis);

		if (glm::abs(f) > 0.001f) // Standard case
		{
			float t1 = (e + bBox.vMin.x) / f; // Intersection with the "left" plane
			float t2 = (e + bBox.vMax.x) / f; // Intersection with the "right" plane
			// t1 and t2 now contain distances betwen ray origin and ray-plane intersections

			// We want t1 to represent the nearest intersection, 
			// so if it's not the case, invert t1 and t2

			if (t1>t2) 
			{
				// swap t1 and t2
				float w = t1; 
				t1 = t2; 
				t2 = w; 
			}
			// tMax is the nearest "far" intersection (amongst the X,Y and Z planes pairs)
			if (t2 < tMax)
				tMax = t2;
			// tMin is the farthest "near" intersection (amongst the X,Y and Z planes pairs)
			if (t1 > tMin)
				tMin = t1;
			// If "far" is closer than "near", then there is NO intersection.
			if (tMax < tMin)
				return false;
		}
		else {
			// Rare case : the ray is almost parallel to the planes, so they don't have any "intersection"
			if (-e + bBox.vMin.x > 0.0f || -e + bBox.vMax.x < 0.0f)
				return false;
		} 	
		// Test intersection with the 2 planes perpendicular to the OBB's Y axis
		// Exactly the same thing as above.
		glm::vec3 yAxis(modelMatrix[1].x, modelMatrix[1].y, modelMatrix[1].z);
		e = glm::dot(yAxis, delta);
		f = glm::dot(rayData.rayDirection, yAxis);

		if (abs(f) > 0.001f)
		{
			float t1 = (e + bBox.vMin.y) / f; // Intersection with the "left" plane
			float t2 = (e + bBox.vMax.y) / f; // Intersection with the "right" plane
			// t1 and t2 now contain distances betwen ray origin and ray-plane intersections

			// We want t1 to represent the nearest intersection, 
			// so if it's not the case, invert t1 and t2

			if (t1>t2)
			{
				// swap t1 and t2
				float w = t1;
				t1 = t2;
				t2 = w;
			}
			// tMax is the nearest "far" intersection (amongst the X,Y and Z planes pairs)
			if (t2 < tMax)
				tMax = t2;
			// tMin is the farthest "near" intersection (amongst the X,Y and Z planes pairs)
			if (t1 > tMin)
				tMin = t1;
			// If "far" is closer than "near", then there is NO intersection.
			if (tMax < tMin)
				return false;
		}
		else {
			if (-e + bBox.vMin.y > 0.0f || -e + bBox.vMax.y < 0.0f)
				return false;
		}
		// Test intersection with the 2 planes perpendicular to the OBB's Z axis
		// Exactly the same thing than above.
		glm::vec3 zAxis(modelMatrix[2].x, modelMatrix[2].y, modelMatrix[2].z);
		e = glm::dot(zAxis, delta);
		f = glm::dot(rayData.rayDirection, zAxis);
		if (abs(f) > 0.001f)
		{
			float t1 = (e + bBox.vMin.z) / f; // Intersection with the "left" plane
			float t2 = (e + bBox.vMax.z) / f; // Intersection with the "right" plane
			// t1 and t2 now contain distances betwen ray origin and ray-plane intersections

			// We want t1 to represent the nearest intersection, 
			// so if it's not the case, invert t1 and t2

			if (t1>t2)
			{
				// swap t1 and t2
				float w = t1;
				t1 = t2;
				t2 = w;
			}
			// tMax is the nearest "far" intersection (amongst the X,Y and Z planes pairs)
			if (t2 < tMax)
				tMax = t2;
			// tMin is the farthest "near" intersection (amongst the X,Y and Z planes pairs)
			if (t1 > tMin)
				tMin = t1;
			// If "far" is closer than "near", then there is NO intersection.
			if (tMax < tMin)
				return false;
		}
		else {
			if (-e + bBox.vMin.z > 0.0f || -e + bBox.vMax.z < 0.0f)
				return false;
		}

		//intersection_distance = tMin;
		return true;

		/*
				// The ModelMatrix transforms :
				// - the mesh to its desired position and orientation
				// - but also the AABB (defined with aabb_min and aabb_max) into an OBB
				glm::mat4 RotationMatrix = glm::toMat4(orientations[i]);
				glm::mat4 TranslationMatrix = translate(mat4(), positions[i]);
				glm::mat4 ModelMatrix = TranslationMatrix * RotationMatrix;
		*/
	}

	static bool collisionTest(const bBox& bBox1, glm::vec3& position1, const bBox& bBox2, const glm::vec3& posittion2)
	{
		//bBox1
		bBox temp1 = bBox1;
		temp1.vMax += position1;
		temp1.vMin += position1;
		//bBox2
		bBox temp2 = bBox2;
		temp2.vMax += posittion2;
		temp2.vMin += posittion2;
		//Collision test
		return(
			temp1.vMax.x > temp2.vMin.x &&
			temp1.vMin.x < temp2.vMax.x &&
			temp1.vMax.y > temp2.vMin.y &&
			temp1.vMin.y < temp2.vMax.y &&
			temp1.vMax.z > temp2.vMin.z &&
			temp1.vMin.z < temp2.vMax.z);
	}

	static void collisionResponse(const bBox& bBox1, Transform& playerTransform, const bBox& bBox2, const glm::vec3& posittion2)
	{
		//bBox1
		bBox temp1 = bBox1;
		temp1.vMax += playerTransform.position;
		temp1.vMin += playerTransform.position;
		//bBox2
		bBox temp2 = bBox2;
		temp2.vMax += posittion2;
		temp2.vMin += posittion2;

		glm::vec3 faces[6] =
		{
			glm::vec3(-1, 0, 0),	// left
			glm::vec3(1, 0, 0),		// right
			glm::vec3(0, -1, 0),	// bottom
			glm::vec3(0, 1, 0),		// top
			glm::vec3(0, 0, -1),	// far
			glm::vec3(0, 0, 1)		// close
		};

		float distance[6] = 
		{
			(temp2.vMax.x - temp1.vMin.x), // distance of box 'b' to face on 'left' side of 'a'.
			(temp1.vMax.x - temp2.vMin.x), // distance of box 'b' to face on 'right' side of 'a'.
			(temp2.vMax.y - temp1.vMin.y), // distance of box 'b' to face on 'bottom' side of 'a'.
			(temp1.vMax.y - temp2.vMin.y), // distance of box 'b' to face on 'top' side of 'a'.
			(temp2.vMax.z - temp1.vMin.z), // distance of box 'b' to face on 'far' side of 'a'.
		    (temp1.vMax.z - temp2.vMin.z) // distance of box 'b' to face on 'near' side of 'a'.
		};
		// scan each face, make sure the box intersects,
		// and take the face with least amount of intersection
		// as the collided face.
		float distanceCollision = 100.f;
		int faceCollision = -1;

		glm::vec3 normalOfCollision; // set value by velocity vector?

		for (int i = 0; i < 6; i++)
		{
			// box does not intersect face. So boxes don't intersect at all.
			if (distance[i] < 0.0f)
				return;
			// face cannot be used for collision response as it is 'blocked'.
			/*
			if (ignore_faces & (1 << i))
				continue;
			*/
			// face of least intersection depth. That's our candidate.
			if (distance[i] < distanceCollision)
			{
				faceCollision = i;
				normalOfCollision = faces[i];
				distanceCollision = distance[i];
			}
		}
		playerTransform.position += -1.f * glm::vec3(distanceCollision * normalOfCollision.x, distanceCollision * normalOfCollision.y, distanceCollision * normalOfCollision.z);
	}
};