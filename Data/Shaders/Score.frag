varying vec2 v_Uv;
uniform sampler2D u_MainTex;

void main()
{
	vec4 colour = texture2D( u_MainTex, v_Uv );

	gl_FragColor = colour;
}
