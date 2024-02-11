#shader vertex
#version 460 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 tex_coord;

out vec2 v_tex_coords;

// Model view proejction matrix
uniform mat4 u_MVP;

void main() {
   gl_Position = u_MVP * position;
   v_tex_coords = tex_coord;
}

#shader fragment
#version 460 core

layout(location = 0) out vec4 color;

in vec2 v_tex_coords;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

// Colour matcher
uniform vec3 target_colour;
uniform vec3 distance_ratios_texOverride;

float distance_to_colour;
float ratios_distance;

bool matches;

void main() {
   vec4 tex_color = texture(u_Texture, v_tex_coords);

   distance_to_colour = sqrt(pow(tex_color.x-target_colour.x, 2) + pow(tex_color.y-target_colour.y, 2) + pow(tex_color.z-target_colour.z, 2));
   ratios_distance = sqrt(
      pow(target_colour.x/target_colour.y - tex_color.x/tex_color.y, 2) +
      pow(target_colour.y/target_colour.z - tex_color.y/tex_color.z, 2)
   );
   if (distance_to_colour < distance_ratios_texOverride.x && ratios_distance < distance_ratios_texOverride.y) {
      color = u_Color*(1-distance_ratios_texOverride.z)+tex_color*distance_ratios_texOverride.z;
   } else {
      color = tex_color;
   }

}
