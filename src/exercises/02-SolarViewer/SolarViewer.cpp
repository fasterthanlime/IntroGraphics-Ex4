//=============================================================================
//                                                                            
//   Exercise code for the lecture
//                                                                            
//=============================================================================
//=============================================================================
//
//  CLASS MeshViewer - IMPLEMENTATION
//
//=============================================================================

//== INCLUDES =================================================================

#include "SolarViewer.h"
#include "../../utils/Mesh3DReader.h"

//== IMPLEMENTATION ========================================================== 


SolarViewer::
SolarViewer(const char* _title, int _width, int _height)
: TrackballViewer(_title, _width, _height)
{
  init();
}



//-----------------------------------------------------------------------------


void 
SolarViewer::
init()
{
  // initialize parent
  TrackballViewer::init();

  // set camera to look at world coordinate center
  set_scene_pos(Vector3(0.0, 0.0, 0.0), 2.0);
	
	// load mesh shader
	m_meshShaderDiffuse.create("diffuse.vs", "diffuse.fs");
	m_meshShaderTexture.create("tex.vs","tex.fs");
	
	m_showTextureStars = false;
	m_showTextureSun = false;
	m_showTextureEarth = false;
	m_showTextureMoon = false;
	
	currentTime = 0.0;
	isWatchOn = false;
	
	daysPerMiliSecond = 1 / 150.0;
	totalDaysElapsed = 0;
	m_geocentric = false;
	m_recSunlightInt = 1.0;
	
	m_moonScale = 50.0;
	m_earthScale = m_moonScale * 2.0;
	m_sunScale = m_moonScale * 5.0;
	m_starsScale = m_moonScale * 100.0;
	
	m_earthTrans = m_sunScale + 1250;
	m_moonTrans = m_earthTrans + 300;
	
	m_starsScale = m_moonScale * 100.0;
	
	srand((unsigned)time(NULL));
	// Mercury
	m_PlanetsScale[0] = m_earthScale * 1.0;
	m_PlanetsTranslate[0] = m_earthTrans*0.4*Vector3(((float)rand()/RAND_MAX)-0.5,0,((float)rand()/RAND_MAX)-0.5).normalize();
	m_PlanetsYear[0] = 88.0;
	
	// Venus
	m_PlanetsScale[1] = m_earthScale * 1.0;
	m_PlanetsTranslate[1] = m_earthTrans*0.7*Vector3(((float)rand()/RAND_MAX)-0.5,0,((float)rand()/RAND_MAX)-0.5).normalize();
	m_PlanetsYear[1] = 224.0;
	
	// Mars
	m_PlanetsScale[2] = m_earthScale * 1.0;
	m_PlanetsTranslate[2] = m_earthTrans*1.5*Vector3(((float)rand()/RAND_MAX)-0.5,0,((float)rand()/RAND_MAX)-0.5).normalize();
	m_PlanetsYear[2] = 780.0;
	
	// Jupiter
	m_PlanetsScale[3] = m_earthScale * 1.5;
	m_PlanetsTranslate[3] = m_earthTrans*5.2*0.5*Vector3(((float)rand()/RAND_MAX)-0.5,0,((float)rand()/RAND_MAX)-0.5).normalize();
	m_PlanetsYear[3] = 4330;
	
	// Saturn
	m_PlanetsScale[4] = m_earthScale * 1.0;
	m_PlanetsTranslate[4] = m_earthTrans*9.5*0.4*Vector3(((float)rand()/RAND_MAX)-0.5,0,((float)rand()/RAND_MAX)-0.5).normalize();
	m_PlanetsYear[4] = 10000;
	
	// Uranus
	m_PlanetsScale[5] = m_earthScale * 1.0;
	m_PlanetsTranslate[5] = m_earthTrans*19.6*0.25*Vector3(((float)rand()/RAND_MAX)-0.5,0,((float)rand()/RAND_MAX)-0.5).normalize();
	m_PlanetsYear[5] = 30000.0;
	
	// Neptune
	m_PlanetsScale[6] = m_earthScale * 1.0;
	m_PlanetsTranslate[6] = m_earthTrans*30.0*0.2*Vector3(((float)rand()/RAND_MAX)-0.5,0,((float)rand()/RAND_MAX)-0.5).normalize();
	m_PlanetsYear[6] = 60000.0;
	
	// Pluto
	m_PlanetsScale[7] = m_earthScale * 0.5;
	m_PlanetsTranslate[7] = m_earthTrans*40.0*0.2*Vector3(((float)rand()/RAND_MAX)-0.5,0,((float)rand()/RAND_MAX)-0.5).normalize();
	m_PlanetsYear[7] = 90600.0;
	
	
}


//-----------------------------------------------------------------------------

void
SolarViewer::
load_mesh(const std::string& filenameObj, MeshType type)
{
	Vector3 bbmin, bbmax;
	double radius;
	Vector3 center;
	switch(type)
	{
		case STARS:
			// load mesh from obj
			Mesh3DReader::read( filenameObj, m_Stars);
			
			// calculate normals
			if(!m_Stars.hasNormals())
				m_Stars.calculateVertexNormals();
			
			
			m_Stars.calculateBoundingBox(bbmin, bbmax);
			
			radius = 0.5*(bbmin - bbmax).length();
			center = 0.5*(bbmin + bbmax);
			
			m_Stars.scaleObject(Vector3(m_starsScale, m_starsScale, m_starsScale));
			radius *= 700.0;
			
			set_scene_pos(m_Stars.origin(), m_earthTrans*3.3);
			
			m_showTextureStars = m_Stars.hasUvTextureCoord();
			
			break;
		case SUN:
			// load mesh from obj
			Mesh3DReader::read( filenameObj, m_Sun);
			
			// calculate normals
			if(!m_Sun.hasNormals())
				m_Sun.calculateVertexNormals();
			
			//Exercise 4.2: Scale the sun using the attribute m_sunScale
			m_Sun.scaleWorld(Vector3(m_sunScale,m_sunScale,m_sunScale));
			
			//Exercise 4.4: Set the light position to the center of the sun 
			m_light.translateWorld(m_Sun.origin());

			m_showTextureSun = m_Sun.hasUvTextureCoord();
			break;
		case EARTH:
			// load mesh from obj
			Mesh3DReader::read( filenameObj, m_Earth);
			
			// calculate normals
			if(!m_Earth.hasNormals())
				m_Earth.calculateVertexNormals();
			
			//Exercise 4.2: Scale and translate the world using the attributes m_earthScale and m_earthTrans
			m_Earth.scaleObject(Vector3(m_earthScale,m_earthScale,m_earthScale));
			m_Earth.translateWorld(Vector3(m_earthTrans,0,0));
		
			m_showTextureEarth = m_Earth.hasUvTextureCoord();
			break;
		case MOON:
			// load mesh from obj
			Mesh3DReader::read( filenameObj, m_Moon);
			
			// calculate normals
			if(!m_Moon.hasNormals())
				m_Moon.calculateVertexNormals();
			
			//Exercise 4.2: Scale and translate the moon using the attributes m_moonScale and m_moonTrans
			m_Moon.scaleObject(Vector3(m_moonScale,m_moonScale,m_moonScale));
			m_Moon.translateWorld(Vector3(m_moonTrans,0,0));
			
			m_showTextureMoon = m_Moon.hasUvTextureCoord();
			break;
		case MERCURY:
		case VENUS:
		case MARS:
		case JUPITER:
		case SATURN:
		case URANUS:
		case NEPTUNE:
		case PLUTO: {
			int index = (type-MERCURY);
			Mesh3DReader::read( filenameObj, m_Planets[index] );
			if(!m_Planets[index].hasNormals())
				m_Planets[index].calculateVertexNormals();
			
			//Exercise 4.6: Scale and translate the planets
			break;
		}
		default:
			break;
	}
	
}

//-------------------------------------------------------------------------------

void
SolarViewer::
keyboard(int key, int x, int y)
{
	switch (key)
	{			
		case 'h':
			printf("Help:\n");
			printf("'h'\t-\thelp\n");
			printf("'t'\t-\ttoggle texture\n");
			printf("'arrow keys\t-\tchange speed of rotation\n");
			break;
		case 't':
			m_showTextureStars = !m_showTextureStars;
			if(!m_Stars.hasUvTextureCoord()) m_showTextureStars = false;
			
			m_showTextureSun = !m_showTextureSun;
			if(!m_Sun.hasUvTextureCoord()) m_showTextureSun = false;
			
			m_showTextureEarth = !m_showTextureEarth;
			if(!m_Earth.hasUvTextureCoord()) m_showTextureEarth = false;
			
			m_showTextureMoon = !m_showTextureMoon;
			if(!m_Moon.hasUvTextureCoord()) m_showTextureMoon = false;
			break;
		case 'g':
			m_geocentric = !m_geocentric;
			break;
		case ' ':
			if(isWatchOn)
			{
				watch.stop();
				currentTime = 0.0;
			}
			else 
			{
				watch.start();
			}

			isWatchOn = !isWatchOn;
			break;
		default:
			TrackballViewer::special(key, x, y);
			break;
	}
	
	glutPostRedisplay();
}

//-----------------------------------------------------------------------------


void
SolarViewer::
special(int key, int x, int y)
{
	switch (key)
	{			
		case GLUT_KEY_UP:
			daysPerMiliSecond += 0.001;
			if(daysPerMiliSecond > 0.1)
				daysPerMiliSecond = 0.1;
			break;
		case GLUT_KEY_DOWN:
			daysPerMiliSecond -= 0.001;
			if(daysPerMiliSecond < 0.001)
				daysPerMiliSecond = 0.001;
			break;
		default:
			TrackballViewer::special(key, x, y);
			break;
	}
	
	glutPostRedisplay();
}


//-----------------------------------------------------------------------------

void SolarViewer::idle()
{
	if(isWatchOn)
	{
		float prevTime = currentTime;
		currentTime = watch.stop();
		float daysElapsed = daysPerMiliSecond * (currentTime-prevTime);
		totalDaysElapsed += daysElapsed;
		
		//Exercise 4.3 Rotate the earth and the moon
		Vector3 up(0, 1, 0);
		
		const float SUN_SELF_PERIOD = 24.47;
		const float EARTH_SELF_PERIOD = 1.0;
		const float EARTH_AROUND_SUN_PERIOD = 365.0;
		const float MOON_AROUND_EARTH = 29.53;
		
		m_Sun.  rotateObject(up, daysElapsed / SUN_SELF_PERIOD * 2 * M_PI);		
		m_Earth.rotateObject(up, daysElapsed / EARTH_SELF_PERIOD * 2 * M_PI);
		m_Earth.rotateAroundAxisWorld(m_Sun.origin(), up, daysElapsed / EARTH_AROUND_SUN_PERIOD * 2 * M_PI);
		m_Moon. rotateAroundAxisWorld(m_Sun.origin(), up, daysElapsed / EARTH_AROUND_SUN_PERIOD * 2 * M_PI);
		m_Moon. rotateAroundAxisWorld(m_Earth.origin(), up, daysElapsed / MOON_AROUND_EARTH * 2 * M_PI);
		
		//Exercise 4.6: Rotate the planets

		glutPostRedisplay();
	}
}

//-----------------------------------------------------------------------------

void 
SolarViewer::
draw_scene(DrawMode _draw_mode)
{
	Vector3 sunToEarthVector = m_Earth.origin() - m_Sun.origin();
	
	//Exercise 4.5: Transform the camera so that the earth becomes the center of rotation
	if(m_geocentric)
	{	
	}
	
	// clear screen
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	
	m_meshShaderTexture.bind(); 
	
	// set parameters
	m_meshShaderTexture.setMatrix4x4Uniform("worldcamera", m_camera.getTransformation().Inverse());
	m_meshShaderTexture.setMatrix4x4Uniform("projection", m_camera.getProjectionMatrix());
	
	glColor3f(1,1,1);
	//stars
	glDisable(GL_DEPTH_TEST);
	m_Stars.setIdentity();
	m_Stars.scaleObject(Vector3(m_starsScale, m_starsScale, m_starsScale));
	m_Stars.translateWorld(Vector3(m_camera.origin()));
	m_meshShaderTexture.setMatrix4x4Uniform("modelworld", m_Stars.getTransformation() );
	m_Stars.getMaterial(0).m_diffuseTexture.bind();
	//printf("tex id: %d %d\n", m_Stars.getMaterial(0).m_diffuseTexture.getID(), m_Stars.getMaterial(0).m_diffuseTexture.getLayer());
	m_meshShaderTexture.setIntUniform("texture", m_Stars.getMaterial(0).m_diffuseTexture.getLayer());
	draw_object(m_meshShaderTexture, m_Stars);
	glEnable(GL_DEPTH_TEST);
	
	//sun
	m_meshShaderTexture.setMatrix4x4Uniform("modelworld", m_Sun.getTransformation() );
	m_Sun.getMaterial(0).m_diffuseTexture.bind();
	m_meshShaderTexture.setIntUniform("texture", m_Sun.getMaterial(0).m_diffuseTexture.getLayer());
	draw_object(m_meshShaderTexture, m_Sun);
	
	m_meshShaderTexture.unbind();
	
	//-------------------------------
	
	m_meshShaderDiffuse.bind();
	
	//Exercise 4.4: Calculate the light position in camera coordinates
	Vector3 lightPosInCamera(0.0,0.0,0.0);
	lightPosInCamera=m_camera.getTransformation().Inverse()*m_light.origin();

	m_meshShaderDiffuse.setMatrix4x4Uniform("worldcamera", m_camera.getTransformation().Inverse());
	m_meshShaderDiffuse.setMatrix4x4Uniform("projection", m_camera.getProjectionMatrix());
	m_meshShaderDiffuse.setMatrix3x3Uniform("worldcameraNormal", m_camera.getTransformation().Transpose());
	m_meshShaderDiffuse.setVector3Uniform("lightposition", lightPosInCamera.x, lightPosInCamera.y, lightPosInCamera.z );
	m_meshShaderDiffuse.setVector3Uniform("lightcolor", m_recSunlightInt, m_recSunlightInt, m_recSunlightInt);
	
	//Exercise 4.4: Setup the indirect lighting
	//calculate the light position and intensity from the moon to the earth
	float moonLightIntensity = 0.0;
	//cos (V,W) = V*W / |V| * |W|
	Vector3 moonSun = (m_Sun.origin() - m_Moon.origin()).normalize();
	Vector3 moonEarth = (m_Earth.origin() - m_Moon.origin()).normalize();
	float cosAngleM = moonSun.dot(moonEarth) / moonSun.length() * moonEarth.length();
	float angleRadM = acos(cosAngleM);
	//float angle = angleRadM*180/M_PI;
	moonLightIntensity = abs(sin(angleRadM)/2) * m_recSunlightInt; 
	
	Vector3 moonLightPosInCamera = m_camera.getTransformation().Inverse()*m_Moon.origin();
	
	//printf("angle : %f \n", angle);
	//printf("sin : %f \n", abs(sin(angleRad)));
	//printf("light : %f \n", abs(sin(angleRad)/2));
	
	//calculate the light position and intensity from the earth to the moon
	float earthLightIntensity = 0.0;
	//cos (V,W) = V*W / |V| * |W|
	Vector3 earthSun = (m_Sun.origin() - m_Earth.origin()).normalize();
	Vector3 earthMoon = (m_Moon.origin() - m_Earth.origin()).normalize();
	float cosAngleE = earthSun.dot(earthMoon) / earthSun.length() * earthMoon.length();
	float angleRadE = acos(cosAngleE);
	
	earthLightIntensity = abs(sin(angleRadE)/2) * m_recSunlightInt; 

	Vector3 earthLightPosInCamera = m_camera.getTransformation().Inverse()*m_Earth.origin();
	
	m_meshShaderDiffuse.setVector3Uniform("indirectlightcolor", moonLightIntensity, moonLightIntensity, moonLightIntensity);
	m_meshShaderDiffuse.setVector3Uniform("indirectlightposition", moonLightPosInCamera.x, moonLightPosInCamera.y, moonLightPosInCamera.z);
	
	//earth
	m_meshShaderDiffuse.setMatrix4x4Uniform("modelworld", m_Earth.getTransformation() );
	m_meshShaderDiffuse.setMatrix3x3Uniform("modelworldNormal", m_Earth.getTransformation().Inverse().Transpose());
	draw_object(m_meshShaderDiffuse, m_Earth, m_showTextureEarth);

	m_meshShaderDiffuse.setVector3Uniform("indirectlightcolor", earthLightIntensity, earthLightIntensity, earthLightIntensity);
	m_meshShaderDiffuse.setVector3Uniform("indirectlightposition", earthLightPosInCamera.x, earthLightPosInCamera.y, earthLightPosInCamera.z);
	
	//moon
	m_meshShaderDiffuse.setMatrix4x4Uniform("modelworld", m_Moon.getTransformation() );
	m_meshShaderDiffuse.setMatrix3x3Uniform("modelworldNormal", m_Moon.getTransformation().Inverse().Transpose());
	draw_object(m_meshShaderDiffuse, m_Moon, m_showTextureMoon);
	
	// other planets
	for(int i =0; i < 8; i++) {
		// no indirect lighting
		m_meshShaderDiffuse.setMatrix4x4Uniform("modelworld", m_Planets[i].getTransformation() );
		m_meshShaderDiffuse.setMatrix3x3Uniform("modelworldNormal", m_Planets[i].getTransformation().Inverse().Transpose());
		m_meshShaderDiffuse.setVector3Uniform("indirectlightcolor", 0, 0, 0);
		draw_object(m_meshShaderDiffuse, m_Planets[i], m_showTextureEarth);
	}
	
	m_meshShaderDiffuse.unbind();

	//Exercise 4.5: Transform the camera back to its original position
	if(m_geocentric) 
	{
	}
}


void SolarViewer::draw_object(Shader& sh, Mesh3D& mesh)
{
	
	sh.setMatrix4x4Uniform("modelworld", mesh.getTransformation() );
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glVertexPointer( 3, GL_DOUBLE, 0, mesh.getVertexPointer() );
	glNormalPointer( GL_DOUBLE, 0, mesh.getNormalPointer() );
	glTexCoordPointer( 2, GL_DOUBLE, 0, mesh.getUvTextureCoordPointer() );
	
	for(unsigned int i = 0; i < mesh.getNumberOfParts(); i++)
	{
		glDrawElements( GL_TRIANGLES, mesh.getNumberOfFaces(i)*3, GL_UNSIGNED_INT, mesh.getVertexIndicesPointer(i) );
	}
	
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
}

void SolarViewer::draw_object(Shader& sh, Mesh3D& mesh, bool showTexture)
{
	
	sh.setMatrix4x4Uniform("modelworld", mesh.getTransformation() );
			
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	if(showTexture)
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			
	glVertexPointer( 3, GL_DOUBLE, 0, mesh.getVertexPointer() );
	glNormalPointer( GL_DOUBLE, 0, mesh.getNormalPointer() );
	if(showTexture)
		glTexCoordPointer( 2, GL_DOUBLE, 0, mesh.getUvTextureCoordPointer() );
			
	for(unsigned int i = 0; i < mesh.getNumberOfParts(); i++)
	{
		sh.setIntUniform("useTexture", showTexture && mesh.getMaterial(i).hasDiffuseTexture());
		sh.setVector3Uniform("diffuseColor", 
							 mesh.getMaterial(i).m_diffuseColor.x, 
							 mesh.getMaterial(i).m_diffuseColor.y, 
							 mesh.getMaterial(i).m_diffuseColor.z );
		sh.setFloatUniform("specularExp", mesh.getMaterial(i).m_specularExp);
		if(showTexture && mesh.getMaterial(i).hasDiffuseTexture())
		{
			mesh.getMaterial(i).m_diffuseTexture.bind();
			sh.setIntUniform("texture", mesh.getMaterial(i).m_diffuseTexture.getLayer());
		}
		glDrawElements( GL_TRIANGLES, mesh.getNumberOfFaces(i)*3, GL_UNSIGNED_INT, mesh.getVertexIndicesPointer(i) );
		if(showTexture && mesh.getMaterial(i).hasDiffuseTexture())
		{
			mesh.getMaterial(i).m_diffuseTexture.unbind();
		}
	}
			
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	if(showTexture)
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
}


//=============================================================================
