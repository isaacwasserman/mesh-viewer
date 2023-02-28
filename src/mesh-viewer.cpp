//--------------------------------------------------
// Author: Isaac Wasserman
// Date: 02/28/2023
// Description: Views PLY files
//--------------------------------------------------

#include <cmath>
#include <string>
#include <vector>
#include "agl/window.h"
#include "plymesh.h"
#include "osutils.h"

using namespace std;
using namespace glm;
using namespace agl;

class MeshViewer : public Window {
public:
   MeshViewer() : Window() {
   }

   void setup() {
      reloadMesh();
   }

   void mouseMotion(int x, int y, int dx, int dy) {
      if(left_mousedown && shiftdown){
         radius = std::max(1.0f, radius - dy * 0.1f);
      }
      if(left_mousedown && !shiftdown){
         azimuth = azimuth + dx * 0.01f;
         elevation = elevation + dy * 0.01f;
      }
   }

   void mouseDown(int button, int mods) {
      if(button == GLFW_MOUSE_BUTTON_LEFT){
         left_mousedown = true;
      }
      if(button == GLFW_MOUSE_BUTTON_RIGHT){
         right_mousedown = true;
      }
      if(button == GLFW_MOUSE_BUTTON_MIDDLE){
         right_mousedown = true;
      }
   }

   void mouseUp(int button, int mods) {
      if(button == GLFW_MOUSE_BUTTON_LEFT){
         left_mousedown = false;
      }
      if(button == GLFW_MOUSE_BUTTON_RIGHT){
         right_mousedown = false;
      }
      if(button == GLFW_MOUSE_BUTTON_MIDDLE){
         right_mousedown = false;
      }
   }

   void scroll(float dx, float dy) {
      radius = std::max(1.0f, radius - dy * 0.1f);
   }

   void keyDown(int key, int mods) {
      if(key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT){
         shiftdown = true;
      }
   }

   void keyUp(int key, int mods) {
      // n
      if(key == 78){
         meshIndex = (meshIndex + 1) % meshPaths.size();
         reloadMesh();
      }
      // p
      if(key == 80){
         meshIndex = (meshIndex - 1) % meshPaths.size();
         reloadMesh();
      }
      if(key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT){
         shiftdown = false;
      }
   }

   void reloadMesh(){
      std::cout << meshPaths[meshIndex] << std::endl;
      string meshPath = "../models/" + meshPaths[meshIndex];
      mesh = PLYMesh(meshPath);
      meshCenter = (mesh.maxBounds() + mesh.minBounds()) / 2.0f;
      vec3 meshWidth = mesh.maxBounds() - mesh.minBounds();
      
      meshScale = 1.5f / std::max(meshWidth.x, std::max(meshWidth.y, meshWidth.z));
   }

   void draw() {
      float aspect = ((float)width()) / height();
      renderer.perspective(glm::radians(fovDegrees), aspect, 0.1f, 50.0f);

      renderer.rotate(vec3(0,0,0));
      renderer.scale(vec3(meshScale));
      renderer.translate(-1.0f * meshCenter);
      renderer.mesh(mesh);

      vec3 eyePos = vec3(
         radius * cos(azimuth) * cos(elevation),
         radius * sin(elevation),
         radius * sin(azimuth) * cos(elevation)
      );

      vec3 camZ = normalize(eyePos - lookPos);
      vec3 camX = normalize(cross(vec3(0,1,0), camZ));
      vec3 camY = normalize(cross(camZ, camX));
      up = camY;

      renderer.lookAt(eyePos, lookPos, up);
   }

protected:
   vector<string> meshPaths = GetFilenamesInDir("../models", "ply");
   float fovDegrees = 60.0f;
   int meshIndex = 0;
   PLYMesh mesh;
   vec3 meshCenter = vec3(0,0,0);
   float meshScale = 1.0f;
   float radius = 5.0f;
   float azimuth = 0.0f;
   float elevation = 0.0f;
   vec3 eyePos = vec3(10, 0, 0);
   vec3 lookPos = vec3(0, 0, 0);
   vec3 up = vec3(0, 1, 0);
   bool left_mousedown = false;
   bool right_mousedown = false;
   bool middle_mousedown = false;
   bool shiftdown = false;
};

int main(int argc, char** argv)
{
   MeshViewer viewer;
   viewer.run();
   return 0;
}

