attribute vec2 a_Position;
attribute vec2 a_Uv;

uniform int u_Xindex;
uniform int u_Yindex;
uniform int u_fPerRow;
uniform int uTextHeight;
uniform int uTextWidth;
uniform float u_NumOfset;

uniform int u_fPerCol;


float tileWidth = uTextWidth / u_fPerRow;
float tileheight = uTextHeight / u_fPerCol;

float scalex = tileWidth / uTextWidth;
float scaley = tileheight / uTextHeight;

float offsetX = (tileWidth * u_Xindex) / uTextWidth;
float offsetY = (tileheight * u_Yindex) / uTextHeight;


varying vec2 v_Uv;


void main()
{

vec2 position = a_Position;
	position.x += u_NumOfset;





  gl_Position = vec4( position, 0, 1 );


	vec2 uv = vec2((a_Uv.x * scalex) + offsetX, ((a_Uv.y - 1) * scaley) - offsetY);

	
	v_Uv = uv;
}
