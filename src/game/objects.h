struct obstacle {
	short size;
	void* eventTouch; /*Event - Player touches block, only triggers once. If not used, pass void*/
	union
	{
		short grid1[STAGE_HEIGHT][1];
		short grid2[STAGE_HEIGHT][2];
		short grid3[STAGE_HEIGHT][3];
	};
};

struct obstacle stageObstacles[1] = {
	{
		.size=16,
		.eventTouch=0,
		.grid = {
			{0},
			{0},
			{0},
			{0},
			{0},
			{0},
			{0},
			{0},
			{0}
		}
	}
};



/*
void* stage_obstacles[] = {
	struct obstacle flat* = {
		.size = 16,
		.event_touch = 0,
		.grid = {
			{0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0}
		};
	}
};
*/