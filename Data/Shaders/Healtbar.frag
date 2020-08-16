
uniform sampler2D u_Texture;
uniform float u_HealthPercent;
uniform vec4 u_HealthBarColor;
varying vec2 v_UV;
varying vec2 v_PosPercent;

void main()
{ 
    vec4 u_EmptyHealthBarColor = vec4(0.0f,1.0f,0.0f,1.0f);
    if(v_PosPercent.x > u_HealthPercent)
		{
		gl_FragColor = u_HealthBarColor;
		}
		else
		{
			vec4 diff = u_HealthBarColor - u_EmptyHealthBarColor;
			gl_FragColor = u_EmptyHealthBarColor + (diff*v_PosPercent.x);
		}
}
