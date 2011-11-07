varying vec3 normal, lightDir, indirectLightDir;

uniform int useTexture;
uniform sampler2D texture;
uniform vec3 diffuseColor;
uniform float specularExp;
uniform vec3 indirectlightcolor;
uniform vec3 lightcolor;
		
void main()
{	
	//Exercise 4.4: Calculate the reflected intensities for the direct sun light (using lightDir and lightcolor) 
	//and indirect light (using indirectLightDir and indirectlightcolor)
		
	vec3 color = vec3(0,0,0);
	vec3 indcolor = vec3(0,0,0);


        if (dot(normal,lightDir) > 0.0)
                color = lightcolor * diffuseColor * dot(normal,lightDir);
         
        if (dot(normal,indirectLightDir) > 0.0)
                indcolor = indirectlightcolor * diffuseColor * dot(normal,indirectLightDir);
        
        vec4 finalcolor = vec4(color, 1.0) + vec4(indcolor, 1.0);

        if (useTexture == 0) {
          gl_FragColor = finalcolor;
	} else {
          gl_FragColor = texture2D(texture, gl_TexCoord[0].xy) * finalcolor;
        }
	
	
	// need this line so OpenGL doesn't optimize out the variables -- remove in your solution
	useTexture; texture; indirectlightcolor; specularExp;
}
