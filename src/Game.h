class Game {
	public:
		Game();
		void start();
	private:
		void create_window();
		void set_opengl_ambient();
		bool intro();
		bool game_loop();
		void game_over();
		void clear_screen();
		void update_screen();
		void trace(const char*);

		sf::RenderWindow sf_window;
		sf::Event event;
		int w_width, w_height;
		bool running;

		float track_speed;

		FILE* fdebug;
		bool debug;

		Timer fps_timer;
		int fps;

		bool inter[8];

		float points;

		struct color colors[4];
		struct color halft;

		sf::Music background;

		//sf::Font font;
};