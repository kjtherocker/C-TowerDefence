
attribute vec2 a_Position;
attribute vec2 a_UV;

uniform vec2 u_Position;
uniform vec2 u_CameraOffset;
uniform vec2 u_ProjectionScale;

uniform vec2 u_UVScale;
uniform vec2 u_UVOffset;

varying vec2 v_UV;

void main()
{
    vec2 objectSpacePos = a_Position;
    vec2 worldSpacePos = objectSpacePos + u_Position;
    vec2 viewSpacePos = worldSpacePos + u_CameraOffset;
    vec2 clipSpacePos = viewSpacePos * u_ProjectionScale;

    gl_Position = vec4( clipSpacePos, 0, 1 );

    v_UV = a_UV * u_UVScale + u_UVOffset;
}
