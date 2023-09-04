include <BOSL/constants.scad>
use <BOSL/shapes.scad>

WIDTH = 30;
HEIGHT = 30;
DEPTH = 40;
WALL = 2;
HOLE = 5;
SCREW_HOLE = 5;

DETAIL = 80;

module powerCutout()
{
	intersection() {
		cylinder(r = 12.7/2, h = 10, $fn = DETAIL);
		cube(size = [30, 11.2, 30], center = true);

	}
}

module shell()
{
	difference() {
		cuboid(size = [WIDTH, DEPTH, HEIGHT], fillet = 1, edges = EDGES_X_ALL + EDGES_LEFT, center = false, $fn = DETAIL);
		translate(v = [WALL, WALL, WALL])
			cube(size = [WIDTH * 2, DEPTH - 2 * WALL, HEIGHT - 2 * WALL]);

	}
}

module bracket()
{
	difference()
	{
		scale(v = [0.65, 1, 1])
			cylinder(r = DEPTH / 2, h = WALL, $fn = DETAIL);
		translate(v = [0, -25, -25])
			cube(size = 50, 50, 50);
		translate(v = [DEPTH / -4 * 0.65, 0, -1])
			cylinder(r = SCREW_HOLE / 2, h = WALL * 2, $fn = DETAIL);
	}
}

module upperBracket()
{
 	translate(v = [WIDTH - WALL, DEPTH / 2, HEIGHT - WALL])
 	rotate(a = [0, 90, 0])
 	bracket();
}

module lowerBracket()
{
 	translate(v = [WIDTH, DEPTH / 2, WALL])
 	rotate(a = [0, -90, 0])
 	bracket();
}

module body()
{
	union() {
		shell();
		upperBracket();
		lowerBracket();
	}
}

module connectorHole()
{
	translate(v = [WIDTH / 2, -5, HEIGHT / 2])
		rotate(a = [-90, 0, 0])
			powerCutout();
}

module wireHole()
{
	translate(v = [WIDTH / 2, DEPTH - 5, HEIGHT / 2])
		rotate(a = [-90, 0, 0])
			cylinder(r = HOLE / 2, h = 10, $fn = DETAIL);

}

module logo()
{
     translate(v = [0, DEPTH - 5, HEIGHT - 0.5])
         rotate(a = [0, 0, -90])
            linear_extrude(height = 2, center = true, convexity = 10)
                scale(v = [0.14, 0.14, 0.1])
                    offset(0.01)
                        import("headstone_transparent_vectorized.svg");
}

difference()
{
	body();
	connectorHole();
	wireHole();
	logo();
}
