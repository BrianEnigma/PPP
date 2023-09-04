include <BOSL/shapes.scad>
use <BOSL/shapes.scad>

DETAIL = 50;
TOLERANCE = 0.25;
WALL_WIDTH = 4;
FILLET = 0.75;

SOLENOID_WIDTH = 23 + TOLERANCE;
SOLENOID_HEIGHT = 26 + TOLERANCE;
SOLENOID_DEPTH = 25;
SOLENOID_OPENING_DIAMETER = 22;

SOLENOID_SCREW_HOLE_OFFSET_X = 6;
SOLENOID_SCREW_HOLE_OFFSET_Y = 7.5;
SOLENOID_SCREW_HOLE_DIAMETER = 3.5;

BRACKET_WIDTH = SOLENOID_WIDTH + 40;
BRACKET_HEIGHT = 15;
BRACKET_WALL = 5;
BRAKET_SCREW_HOLE_DIAMETER = 3;

module solenoid_body()
{
        translate(v = [0, 0, SOLENOID_DEPTH / 2 + 5 + WALL_WIDTH])
            cube(size = [SOLENOID_WIDTH, SOLENOID_HEIGHT, SOLENOID_DEPTH + 10], center = true);
}

module screw_hole(offset_x, offset_y)
{
    translate(v = [offset_x, 0, 6 + 7.5 + offset_y])
        translate(v = [0, 0, WALL_WIDTH])
            rotate(a = [90, 0, 0])
                cylinder(r = SOLENOID_SCREW_HOLE_DIAMETER / 2, h = SOLENOID_HEIGHT, $fn = DETAIL);

}

module main_body()
{
    body_width = SOLENOID_WIDTH + WALL_WIDTH * 2;
    body_height = SOLENOID_HEIGHT + WALL_WIDTH * 2;
    body_depth = SOLENOID_DEPTH + WALL_WIDTH;
    difference()
    {
        // Main body
        translate(v = [0, 0, body_depth / 2])
            cuboid(
                size = [body_width, body_height, body_depth],
                fillet = FILLET,
                edges = EDGES_Z_ALL,
                center = true,
                $fn = DETAIL
                );

        // Hole for solenoid slug
        translate(v = [0, 0, -1])
            cylinder(r = SOLENOID_OPENING_DIAMETER / 2, h = body_depth + 10, $fn = DETAIL);
        // Solenoid body
        solenoid_body();
        // Mounting holes
        screw_hole(-1 * SOLENOID_SCREW_HOLE_OFFSET_X, SOLENOID_SCREW_HOLE_OFFSET_Y);
        screw_hole(SOLENOID_SCREW_HOLE_OFFSET_X, -1 * SOLENOID_SCREW_HOLE_OFFSET_Y);
    }
}

module bracket1()
{
    difference()
    {
        // Body
        translate(v = [0, SOLENOID_HEIGHT / 2 + WALL_WIDTH + BRACKET_WALL / -2, BRACKET_HEIGHT / -2 + SOLENOID_DEPTH + WALL_WIDTH])
            cuboid(
                size = [BRACKET_WIDTH, BRACKET_WALL, BRACKET_HEIGHT], 
                fillet = FILLET, 
                edges = EDGES_Z_ALL + EDGES_Y_ALL,
                center = true, 
                $fn = DETAIL);
        // Screw holes
        translate(v = [(BRACKET_WIDTH - SOLENOID_WIDTH) / 1.75, SOLENOID_HEIGHT / 2 - 3, BRACKET_HEIGHT / -2 + SOLENOID_DEPTH + WALL_WIDTH])
            rotate(a = [-90, 0, 0])
                cylinder(r = BRAKET_SCREW_HOLE_DIAMETER / 2, h = BRACKET_WALL * 2, $fn = DETAIL);
        translate(v = [(BRACKET_WIDTH - SOLENOID_WIDTH) / -1.75, SOLENOID_HEIGHT / 2 - 3, BRACKET_HEIGHT / -2 + SOLENOID_DEPTH + WALL_WIDTH])
            rotate(a = [-90, 0, 0])
                cylinder(r = BRAKET_SCREW_HOLE_DIAMETER / 2, h = BRACKET_WALL * 2, $fn = DETAIL);
    }
}

module bracket2()
{
    difference()
    {
        translate(v = [0, SOLENOID_HEIGHT / 2 + WALL_WIDTH + BRACKET_WALL / -2, BRACKET_HEIGHT / -4 + SOLENOID_DEPTH + WALL_WIDTH - BRACKET_HEIGHT + 0.01])
            cuboid(
                size = [SOLENOID_WIDTH + WALL_WIDTH * 2 + BRACKET_HEIGHT, BRACKET_WALL, BRACKET_HEIGHT / 2], 
                center = true, 
                $fn = DETAIL);
        // Arches
        translate(v = [SOLENOID_WIDTH / 2 + WALL_WIDTH + BRACKET_HEIGHT / 2, SOLENOID_HEIGHT / 2 + WALL_WIDTH + BRACKET_WALL / 2, BRACKET_HEIGHT / -2 + SOLENOID_DEPTH + WALL_WIDTH - BRACKET_HEIGHT])
            rotate(a = [90, 0, 0])
                cylinder(r = BRACKET_HEIGHT / 2, h = BRACKET_WALL * 2, $fn = DETAIL);
        translate(v = [SOLENOID_WIDTH / -2 - WALL_WIDTH - BRACKET_HEIGHT / 2, SOLENOID_HEIGHT / 2 + WALL_WIDTH + BRACKET_WALL / 2, BRACKET_HEIGHT / -2 + SOLENOID_DEPTH + WALL_WIDTH - BRACKET_HEIGHT])
            rotate(a = [90, 0, 0])
                cylinder(r = BRACKET_HEIGHT / 2, h = BRACKET_WALL * 2, $fn = DETAIL);
    }
}

module bracket3()
{
    difference()
    {
        translate(v = [0, SOLENOID_HEIGHT / 2 + WALL_WIDTH - BRACKET_HEIGHT / 4 - WALL_WIDTH + 0.01, SOLENOID_DEPTH + WALL_WIDTH - BRACKET_WALL / 2])
            cuboid(
                size = [SOLENOID_WIDTH + WALL_WIDTH * 2 + BRACKET_HEIGHT, BRACKET_HEIGHT / 2, BRACKET_WALL], 
                center = true, 
                $fn = DETAIL);
        translate(v = [0, BRACKET_HEIGHT / -2, 0])
            translate(v = [SOLENOID_WIDTH / -2 - WALL_WIDTH + BRACKET_HEIGHT / -2, SOLENOID_HEIGHT / 2 + WALL_WIDTH - BRACKET_WALL, SOLENOID_DEPTH + WALL_WIDTH / 2 - BRACKET_WALL])
                cylinder(r = BRACKET_HEIGHT / 2, h = BRACKET_WALL * 2, $fn = DETAIL);
        translate(v = [0, BRACKET_HEIGHT / -2, 0])
            translate(v = [SOLENOID_WIDTH / 2 + WALL_WIDTH + BRACKET_HEIGHT / 2, SOLENOID_HEIGHT / 2 + WALL_WIDTH - BRACKET_WALL, SOLENOID_DEPTH + WALL_WIDTH / 2 - BRACKET_WALL])
                cylinder(r = BRACKET_HEIGHT / 2, h = BRACKET_WALL * 2, $fn = DETAIL);
    }
}

module bracket_all()
{
    difference()
    {
        union()
        {
            bracket1();
            bracket2();
            bracket3();
        }
        solenoid_body();
    }
}

module logo()
{
    translate(v = [0, -2, 5])
        translate(v = [SOLENOID_HEIGHT / 2 + WALL_WIDTH / 2, -11, 0])
            rotate(a = [90, 0, 90])
                linear_extrude(height = 2, center = true, convexity = 10)
                    scale(v = [0.1, 0.1, 0.1])
                        offset(0.01)
                            import("headstone_transparent_vectorized.svg");


    translate(v = [0, -2, 5])
        translate(v = [SOLENOID_HEIGHT / -2 + WALL_WIDTH / -2, 11, 0])
            rotate(a = [90, 0, -90])
                linear_extrude(height = 2, center = true, convexity = 10)
                    scale(v = [0.1, 0.1, 0.1])
                        offset(0.01)
                            import("headstone_transparent_vectorized.svg");
}

module main()
{
    difference()
    {
        union()
        {
            main_body();
            bracket_all();
        }
        logo();
    }
}

mode = 1;

if (1 == mode)
{
    main();
} else if (2 == mode)
{
    union()
    {
        main();
        #solenoid_body();
    }
} else if (3 == mode)
{
    intersection()
    {
        main();
        #solenoid_body();
    }
} else if (4 == mode)
{
    bracket_all();
}
