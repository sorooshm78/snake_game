# game_network_snake

snake game

menu
alone bot
player 1 by bots
player 1 by player2
player 1 by player2 by bots

# TODO List War Bots

* Remove blinking by repositioning cursor.
* Add class point:
struct Point
{
	Point();
	Point(int x, int y);
	const int x, y;
	// Point a, b; Bool b = (a == b);
}

* Delete 'is_bot' concept
* Define Enum Color
* Correct 'lenght' to 'length'
* Delete Snake::primitive_size
* Delete Snake::last_move
* Throw exception if conditions are expectenary
* Add: 'Point get_next(Point current, Direction direction)'
* Rename 'Move' to 'Direction' 
* Remove 'to_corruct_direction(direction)'
* Delete direction from move_once
* Ask page for end game

* Reposition 'move' semantic from page to snake.
