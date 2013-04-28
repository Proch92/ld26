#include "ld26.h"

Game::Game() {
	trace("game created\n");

	running = false;
	debug = IS_DEBUG_ENABLED;
	fdebug = fopen("debug.txt", "w");
	if(fdebug == NULL) {
		printf("failed creating debug file\n");
	}

	//if(!font.LoadFromFile("data/font2.ttf"))
	//	trace("unable to load font\n");

	track_speed = 0.02;

	int i;
	for(i=0; i!=8; i++)
		inter[i] = false;

	points = 0;

	srand(time(NULL));

	halft.r = 0;
	halft.g = 0;
	halft.b = 0;
	halft.a = 0.4;

	colors[0].r = 1;
	colors[0].g = 0;
	colors[0].b = 1;
	colors[0].a = 1;

	colors[1].r = 0;
	colors[1].g = 1;
	colors[1].b = 1;
	colors[1].a = 1;

	colors[2].r = 1;
	colors[2].g = 1;
	colors[2].b = 0;
	colors[2].a = 1;

	colors[3].r = 0;
	colors[3].g = 1;
	colors[3].b = 0;
	colors[3].a = 1;

	if (!background.OpenFromFile("data/background.ogg"))
	    trace("background music upload failed\n");
}

void Game::start() {
	trace("game strating\n");

	running = true;

	//create window
	create_window();

	//set opengl ambient
	set_opengl_ambient();

	//run intro
	bool again = true;
	while(again)
		if(intro())
			again = game_loop();
		else return;
}

void Game::create_window() {
	trace("creating window\n");

	w_width = DEFAULT_SCREEN_WIDTH;
	w_height = DEFAULT_SCREEN_HEIGHT;
	sf_window.Create(sf::VideoMode(w_width, w_height, 32), DEFAULT_WINDOW_NAME, sf::Style::Close);

	trace("window created\n");
}
void Game::set_opengl_ambient() {
	trace("setting opengl ambient\n");

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, (GLsizei)w_width, (GLsizei)w_height);
	glOrtho(0, w_width, w_height, 0, -1, 1);

	glMatrixMode( GL_MODELVIEW );
	glClearColor(0.85, 0.85, 0.85, 1.0);
	glLoadIdentity();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable( GL_BLEND );
    glEnable( GL_TEXTURE_2D );
	glPushAttrib( GL_DEPTH_BUFFER_BIT | GL_LIGHTING_BIT );
	glDisable( GL_DEPTH_TEST );
	glDisable( GL_LIGHTING );

	clear_screen();
	update_screen();
}

GLuint load_texture(const char* name) {
	sf::Image image;
	GLuint tex;

	if(!image.LoadFromFile(name))
		printf("unable to load texture\n");

	//load textures
	glGenTextures(1, &tex);

	glBindTexture(GL_TEXTURE_2D, tex);
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	// when texture area is small, bilinear filter the closest mipmap
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
	// when texture area is large, bilinear filter the original
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

	glTexImage2D(GL_TEXTURE_2D, 0, 3, image.GetWidth(), image.GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, image.GetPixelsPtr());

	return tex;
}

bool Game::intro() {
	trace("starting intro\n");

	bool finish = false;

	//load textures
	GLuint tex1 = load_texture("data/tex1.png");

	clear_screen();

	//global tranformations
	glPushMatrix();
	glTranslatef(0, -50, 0);

	int k, i, j;
	for(k=0; k<=100 && !finish; k++) {
		glClear(GL_COLOR_BUFFER_BIT);

		//display intro screen
		for(j=0; j!=4; j++) {
			glPushMatrix();
			glTranslatef(260 - TRACKTHICKNESS + 90, 384, 0.0f);
			glBegin(GL_TRIANGLE_STRIP);
			for(i=180; i>=-180; i--) {
				glColor4f(halft.r, halft.g, halft.b, 0.2);

				double rad = i * DEGTORAD;
				int radius = 210 - (30 * (3 - j));
				glVertex2f(cos(rad)*radius,sin(rad)*radius);
				glVertex2f(cos(rad)*(radius + TRACKTHICKNESS),sin(rad)*(radius + TRACKTHICKNESS));
			}
			glEnd();
			glPopMatrix();
		}

		glPushMatrix();
		glColor4f(0.85, 0.85, 0.85, 1);
		glBegin(GL_QUADS);
			glVertex2f(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT / 2);
			glVertex2f(DEFAULT_SCREEN_WIDTH, 0);
			glVertex2f(0, 0);
			glVertex2f(0, DEFAULT_SCREEN_HEIGHT / 2);
		glEnd();
		glPopMatrix();

		for(j=0; j!=4; j++) {
			glPushMatrix();
			glTranslatef(680, 384, 0.0f);
			glBegin(GL_TRIANGLE_STRIP);
			for(i=180; i<=540; i++) {
				glColor4f(colors[j].r, colors[j].g, colors[j].b, 0.2);

				double rad = i * DEGTORAD;
				int radius = 210 - (30 * j);
				glVertex2f(cos(rad)*radius,sin(rad)*radius);
				glVertex2f(cos(rad)*(radius + TRACKTHICKNESS),sin(rad)*(radius + TRACKTHICKNESS));
			}
			glEnd();
			glPopMatrix();
		}

		//line
		glPushMatrix();
		glBegin(GL_LINES);
			glColor4f(0, 0, 0, 0);
			glVertex2f(680, 384);
			glColor4f(0, 0, 0, 1);
			glVertex2f(DEFAULT_SCREEN_WIDTH - 50, 384);
		glEnd();
		glPopMatrix();

		glPushMatrix();
		float transparency = (float)((float)(100 - k) / 100);
		printf("%f\n", transparency);
		glColor4f(0, 0, 0, transparency);
		glBegin(GL_QUADS);
			glVertex2f(0, 0);
			glVertex2f(0, DEFAULT_SCREEN_HEIGHT);
			glVertex2f(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
			glVertex2f(DEFAULT_SCREEN_WIDTH, 0);
		glEnd();
		glPopMatrix();
		
		update_screen();

		usleep(150);

		while(sf_window.GetEvent(event)) {
			if (event.Type == sf::Event::Closed)
				running = false;
			if(event.Type == sf::Event::KeyPressed) {
				switch(event.Key.Code) {
					case sf::Key::Escape:
						finish = true;
						running = false;
						return false;
						break;
					default:
						finish = true;
				}
			}
		}
	}
	glPopMatrix();

	/*sf::String text;
	text.SetText("prova");
	text.SetPosition(50, 50);
	text.SetFont(font);
	text.SetSize(30);
 	text.SetColor(sf::Color::Red);

 	sf_window.Draw(text);*/

 	//show tutorial

 	glBindTexture(GL_TEXTURE_2D, tex1); //274 * 97 2.82x
 	glPushMatrix();
 	glBegin(GL_QUADS);
 		glTexCoord2f(0.0f, 0.0f); glVertex2f(30, 200);
 		glTexCoord2f(0.0f, 1.0f); glVertex2f(30 + 40, 200);
 		glTexCoord2f(1.0f, 1.0f); glVertex2f(30 + 40 ,200 + 212);
 		glTexCoord2f(1.0f, 0.0f); glVertex2f(30, 200 + 212);
 	glEnd();
 	glPopMatrix();

	update_screen();

	while(!finish) {
		while(sf_window.GetEvent(event)) {
			if (event.Type == sf::Event::Closed)
				running = false;
			if(event.Type == sf::Event::KeyPressed) {
				switch(event.Key.Code) {
					case sf::Key::Escape:
						running = false;
						return false;
						break;
					default:
						finish = true;
				}
			}
		}
	}
	return true;
}

bool Game::game_loop() {
	trace("starting game loop\n");

	background.SetLoop(true);
	background.Play();

	clear_screen();

	bool track[4][360];
	bool half_track[4][360];
	bool key_pressed[4];
	float rotation;
	float h_rotation;

	float transparency = 0;

	bool gameover = false;

	char creating[4];
	int k;
	for(k=0; k!=4; k++) {
		creating[k] = 0;
		key_pressed[k] = false;
	}

	int i, j;
	for(i=0; i!=4; i++)
		for(j=0; j!=360; j++)
			track[i][j] = false;

	for(i=0; i!=4; i++)
		for(j=0; j!=360; j++)
			half_track[i][j] = false;

	rotation = 0;

	fps_timer.start();
	fps = 0;

	//global tranformations
	glPushMatrix();
	glTranslatef(0, -50, 0);

	while(running) {
		//events
		if(!gameover) {
			while(sf_window.GetEvent(event)) {
				if (event.Type == sf::Event::Closed)
					running = false;
				if(event.Type == sf::Event::KeyPressed) {
					switch(event.Key.Code) {
						case sf::Key::Escape:
							running = false;
							return false;
							break;
						case sf::Key::U:
							key_pressed[3] = true;
							break;
						case sf::Key::I:
							key_pressed[2] = true;
							break;
						case sf::Key::O:
							key_pressed[1] = true;
							break;
						case sf::Key::P:
							key_pressed[0] = true;
							break;
					}
				}
				if(event.Type == sf::Event::KeyReleased) {
					switch(event.Key.Code) {
						case sf::Key::U:
							key_pressed[3] = false;
							break;
						case sf::Key::I:
							key_pressed[2] = false;
							break;
						case sf::Key::O:
							key_pressed[1] = false;
							break;
						case sf::Key::P:
							key_pressed[0] = false;
							break;
					}
				}
			}
		}

		glClear(GL_COLOR_BUFFER_BIT);

		//passes notes from half track to track
		for(i=0; i!=4; i++) {
			if(half_track[i][((int)rotation + 180) % 360]) {
				track[i][(int)rotation] = !track[i][(int)rotation];
				half_track[i][(180 + (int)rotation) % 360] = false;
			}
		}

		//create new notes
		for(i=0; i!=4; i++)
			if(creating[i] == 0 && (rand() % 10000) > 9980) {
				creating[i] = (rand() % 21) + 20; //da 20 a 40
			}
			else if (creating[i] > 0) {
				creating[i]--;
				half_track[i][(int)rotation] = true;
			}

		if(!gameover) {//key events
			for(i=0; i!=4; i++) {
				if(track[i][(360 + 180 + (int)rotation) % 360] && !key_pressed[i])
					points--;

				if(key_pressed[i]) {
					bool right = false;
					if(track[i][(360 + 180 + (int)rotation) % 360]) //3 tick di tollerance
						points++;
					else {
						points--;
						right = true;
					}

					track[i][(360 + 181 + (int)rotation) % 360] = true;

					//show effect
					/*float radius = 210 - (30 * i);

					glPushMatrix();
					glBegin(GL_QUADS);
						if(right) glColor4f(colors[i].r, colors[i].g, colors[i].b, 0.75);
						else glColor4f(halft.r, halft.g, halft.b, 0.75);
						glVertex2f(680 + radius, 384);
						if(right) glColor4f(colors[i].r, colors[i].g, colors[i].b, 0.75);
						else glColor4f(halft.r, halft.g, halft.b, 0.75);
						glVertex2f(680 + radius + TRACKTHICKNESS, 384);
						if(right) glColor4f(colors[i].r, colors[i].g, colors[i].b, 0.15);
						else glColor4f(halft.r, halft.g, halft.b, 0.15);
						glVertex2f(680 + radius + TRACKTHICKNESS, 384 - 10);
						if(right) glColor4f(colors[i].r, colors[i].g, colors[i].b, 0.15);
						else glColor4f(halft.r, halft.g, halft.b, 0.15);
						glVertex2f(680 + radius, 384 - 10);
					glEnd();
					glPopMatrix();*/
				}
			}
		}

		for(j=0; j!=4; j++) {
			glPushMatrix();
			glTranslatef(260 - TRACKTHICKNESS + 90, 384, 0.0f);
			glRotatef(rotation, 0, 0, 1);
			glBegin(GL_TRIANGLE_STRIP);
			for(i=180; i>=-180; i--) {
				if(half_track[j][360 - (i + 180)])
					glColor4f(halft.r, halft.g, halft.b, halft.a);
				else
					glColor4f(halft.r, halft.g, halft.b, 0.2);

				double rad = i * DEGTORAD;
				int radius = 210 - (30 * (3 - j));
				glVertex2f(cos(rad)*radius,sin(rad)*radius);
				glVertex2f(cos(rad)*(radius + TRACKTHICKNESS),sin(rad)*(radius + TRACKTHICKNESS));
			}
			glEnd();
			glPopMatrix();
		}

		glPushMatrix();
		glColor4f(0.85, 0.85, 0.85, 1);
		glBegin(GL_QUADS);
			glVertex2f(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT / 2);
			glVertex2f(DEFAULT_SCREEN_WIDTH, 0);
			glVertex2f(0, 0);
			glVertex2f(0, DEFAULT_SCREEN_HEIGHT / 2);
		glEnd();
		glPopMatrix();

		for(j=0; j!=4; j++) {
			glPushMatrix();
			glTranslatef(680, 384, 0.0f);
			glRotatef(rotation, 0, 0, -1);
			glBegin(GL_TRIANGLE_STRIP);
			for(i=180; i<=540; i++) {
				if(track[j][i - 180])
					glColor4f(colors[j].r, colors[j].g, colors[j].b, colors[j].a);
				else
					glColor4f(colors[j].r, colors[j].g, colors[j].b, 0.2);

				double rad = i * DEGTORAD;
				int radius = 210 - (30 * j);
				glVertex2f(cos(rad)*radius,sin(rad)*radius);
				glVertex2f(cos(rad)*(radius + TRACKTHICKNESS),sin(rad)*(radius + TRACKTHICKNESS));
			}
			glEnd();
			glPopMatrix();
		}

		//lines
		glPushMatrix();
		glBegin(GL_LINES);
			glColor4f(0, 0, 0, 0);
			glVertex2f(680, 384);
			glColor4f(0, 0, 0, 1);
			glVertex2f(DEFAULT_SCREEN_WIDTH - 50, 384);
		glEnd();
		glPopMatrix();

		for(i=0; i!=4; i++) {
			if(key_pressed[i]) {
				int radius = 210 - (30 * i);

				glPushMatrix();
				glBegin(GL_LINE_STRIP);
					glColor4f(colors[i].r, colors[i].g, colors[i].b, 0);
					glVertex2f(680 + radius + (TRACKTHICKNESS / 2), 384 - 100);
					glColor4f(colors[i].r, colors[i].g, colors[i].b, 0.7);
					glVertex2f(680 + radius + (TRACKTHICKNESS / 2), 384);
					glColor4f(colors[i].r, colors[i].g, colors[i].b, 0);
					glVertex2f(680 + radius + (TRACKTHICKNESS / 2), 384 + 100);
				glEnd();
				glPopMatrix();
			}
		}

		//points
		float mid = 1 - (float)(points + 1000) / (-MAXMISSES * 2);
		glPushMatrix();
		glBegin(GL_LINES);
			glColor4f(1, 0, 0, 0);
			glVertex2f(100, DEFAULT_SCREEN_HEIGHT - 50);
			glColor4f(1, 0, 0, 1);
			glVertex2f(100 + (mid * 824), DEFAULT_SCREEN_HEIGHT - 50);
			glColor4f(0, 1, 0, 1);
			glVertex2f(100 + (mid * 824), DEFAULT_SCREEN_HEIGHT - 50);
			glColor4f(0, 1, 0, 0);
			glVertex2f(DEFAULT_SCREEN_WIDTH - 100, DEFAULT_SCREEN_HEIGHT - 50);
		glEnd();
		glPopMatrix();

		//game over fading
		if(gameover) {
			glPushMatrix();
			glColor4f(1, 1, 1, transparency);
			glBegin(GL_QUADS);
				glVertex2f(0, 0);
				glVertex2f(0, DEFAULT_SCREEN_HEIGHT);
				glVertex2f(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
				glVertex2f(DEFAULT_SCREEN_WIDTH, 0);
			glEnd();
			glPopMatrix();

			transparency += 0.001;

			track_speed += 0.001;

			if(transparency >= 1) {
				background.Stop();
				running = false;
			}
		}

		update_screen();

		//fps
		long delta_time;
		if(MAXFPS != 0 && (delta_time = fps_timer.get_elapsed_last_call()) <= (1000/MAXFPS)) {
			usleep((1000/MAXFPS) - delta_time);
		}
		if(fps_timer.get_tick() >= 1000) {
			printf("fps = %d\n", fps);
			fps = 0;
			fps_timer.start();
		}
		else fps++;

		rotation = rotation - (delta_time * track_speed);

		if(rotation < 0)
			rotation = 360 - rotation;

		if(!gameover && (points <= MAXMISSES || points >= -MAXMISSES))
			gameover = true;
	}

	glPopMatrix();

	return true;
}

void Game::game_over() {

}

void Game::trace(const char* string) {
	if(!debug) return;

	printf("%s", string);

	if(fdebug != NULL) fprintf(fdebug, "%s", string);
}

void Game::clear_screen() {
	glClear(GL_COLOR_BUFFER_BIT);
	update_screen();
}

void Game::update_screen() {
	sf_window.Display();
}