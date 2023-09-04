include <BOSL/shapes.scad>
use <BOSL/shapes.scad>

DETAIL = 50;
TOLERANCE = 0.25;
WALL_WIDTH = 4;
FILLET = 0.75;

SOLENOID_WIDTH = 25.5 + TOLERANCE;
SOLENOID_HEIGHT = 30 + TOLERANCE;
SOLENOID_DEPTH = 35;
SOLENOID_OPENING_DIAMETER = 24;

SOLENOID_SCREW_HOLE_OFFSET_X = (19.5 / 2);
SOLENOID_SCREW_HOLE_OFFSET_Y1 = 11;
SOLENOID_SCREW_HOLE_OFFSET_Y2 = 29;
SOLENOID_SCREW_HOLE_DIAMETER = 3.5;

BRACKET_WIDTH = SOLENOID_WIDTH * 3 + WALL_WIDTH * 4 + 40;
BRACKET_HEIGHT = 15;
BRACKET_WALL = 5;
BRAKET_SCREW_HOLE_DIAMETER = 5;

// +--+--+--+
// |  |  |  |
// +--+--+--+

module solenoid_body()
{
        translate(v = [0, 0, SOLENOID_DEPTH / 2 + 5 + WALL_WIDTH])
            cube(size = [SOLENOID_WIDTH, SOLENOID_HEIGHT, SOLENOID_DEPTH + 10], center = true);
}

module screw_hole(offset_x, offset_y)
{
    translate(v = [offset_x, 0, offset_y])
        translate(v = [0, 0, WALL_WIDTH])
            rotate(a = [90, 0, 0])
                cylinder(r = SOLENOID_SCREW_HOLE_DIAMETER / 2, h = SOLENOID_HEIGHT, $fn = DETAIL);

}

module main_body()
{
    body_width = SOLENOID_WIDTH * 3 + WALL_WIDTH * 4;
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

        solenoid_offset_values = [-1 * (SOLENOID_WIDTH + WALL_WIDTH), 0, SOLENOID_WIDTH + WALL_WIDTH];
        for (solenoid_offset = [0 : 2])
        {
            translate(v = [solenoid_offset_values[solenoid_offset], 0, 0])
            {
                // Hole for solenoid slug
                translate(v = [0, 0, -1])
                    cylinder(r = SOLENOID_OPENING_DIAMETER / 2, h = body_depth + 10, $fn = DETAIL);
                // Solenoid body
                solenoid_body();
                // Mounting holes
                screw_hole(     SOLENOID_SCREW_HOLE_OFFSET_X, SOLENOID_SCREW_HOLE_OFFSET_Y1);
                screw_hole(-1 * SOLENOID_SCREW_HOLE_OFFSET_X, SOLENOID_SCREW_HOLE_OFFSET_Y1);
                screw_hole(     SOLENOID_SCREW_HOLE_OFFSET_X, SOLENOID_SCREW_HOLE_OFFSET_Y2);
                screw_hole(-1 * SOLENOID_SCREW_HOLE_OFFSET_X, SOLENOID_SCREW_HOLE_OFFSET_Y2);
            }
        }
    }
}

module bracket1()
{
    horizontal_offset = (BRACKET_WIDTH - SOLENOID_WIDTH) / 1.9;
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
        translate(v = [horizontal_offset, SOLENOID_HEIGHT / 2 - 3, BRACKET_HEIGHT / -2 + SOLENOID_DEPTH + WALL_WIDTH])
            rotate(a = [-90, 0, 0])
                cylinder(r = BRAKET_SCREW_HOLE_DIAMETER / 2, h = BRACKET_WALL * 2, $fn = DETAIL);
        translate(v = [-1 * horizontal_offset, SOLENOID_HEIGHT / 2 - 3, BRACKET_HEIGHT / -2 + SOLENOID_DEPTH + WALL_WIDTH])
            rotate(a = [-90, 0, 0])
                cylinder(r = BRAKET_SCREW_HOLE_DIAMETER / 2, h = BRACKET_WALL * 2, $fn = DETAIL);
    }
}

module bracket2()
{
    width = SOLENOID_WIDTH * 3 + WALL_WIDTH * 4 + BRACKET_HEIGHT;
    difference()
    {
        translate(v = [0, SOLENOID_HEIGHT / 2 + WALL_WIDTH + BRACKET_WALL / -2, BRACKET_HEIGHT / -4 + SOLENOID_DEPTH + WALL_WIDTH - BRACKET_HEIGHT + 0.01])
            cuboid(
                size = [width, BRACKET_WALL, BRACKET_HEIGHT / 2], 
                center = true, 
                $fn = DETAIL);
        // Arches
        translate(v = [width / 2, SOLENOID_HEIGHT / 2 + WALL_WIDTH + BRACKET_WALL / 2, BRACKET_HEIGHT / -2 + SOLENOID_DEPTH + WALL_WIDTH - BRACKET_HEIGHT])
            rotate(a = [90, 0, 0])
                cylinder(r = BRACKET_HEIGHT / 2, h = BRACKET_WALL * 2, $fn = DETAIL);
        translate(v = [width / -2, SOLENOID_HEIGHT / 2 + WALL_WIDTH + BRACKET_WALL / 2, BRACKET_HEIGHT / -2 + SOLENOID_DEPTH + WALL_WIDTH - BRACKET_HEIGHT])
            rotate(a = [90, 0, 0])
                cylinder(r = BRACKET_HEIGHT / 2, h = BRACKET_WALL * 2, $fn = DETAIL);
    }
}

module bracket3()
{
    width = SOLENOID_WIDTH * 3 + WALL_WIDTH * 4 + BRACKET_HEIGHT;
    difference()
    {
        translate(v = [0, SOLENOID_HEIGHT / 2 + WALL_WIDTH - BRACKET_HEIGHT / 4 - WALL_WIDTH + 0.01, SOLENOID_DEPTH + WALL_WIDTH - BRACKET_WALL / 4])
            cuboid(
                size = [width, BRACKET_HEIGHT / 2, BRACKET_WALL /2], 
                center = true, 
                $fn = DETAIL);
        translate(v = [0, BRACKET_HEIGHT / -2, 0])
            translate(v = [width / -2, SOLENOID_HEIGHT / 2 + WALL_WIDTH - BRACKET_WALL, SOLENOID_DEPTH + WALL_WIDTH / 2 - BRACKET_WALL])
                cylinder(r = BRACKET_HEIGHT / 2, h = BRACKET_WALL * 2, $fn = DETAIL);
        translate(v = [0, BRACKET_HEIGHT / -2, 0])
            translate(v = [width / 2, SOLENOID_HEIGHT / 2 + WALL_WIDTH - BRACKET_WALL, SOLENOID_DEPTH + WALL_WIDTH / 2 - BRACKET_WALL])
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
        solenoid_offset_values = [-1 * (SOLENOID_WIDTH + WALL_WIDTH), 0, SOLENOID_WIDTH + WALL_WIDTH];
        for (solenoid_offset = [0 : 2])
        {
            translate(v = [solenoid_offset_values[solenoid_offset], 0, 0])
            {
                solenoid_body();
            }
        }
    }
}

module logo()
{
    offset = SOLENOID_WIDTH * 1.5 + WALL_WIDTH * 2;
    translate(v = [0, -2, 8])
        translate(v = [offset, -13, 0])
            rotate(a = [90, 0, 90])
                linear_extrude(height = 2, center = true, convexity = 10)
                    scale(v = [0.12, 0.12, 0.12])
                        offset(0.01)
                            import("headstone_transparent_vectorized.svg");


    translate(v = [0, -1.5, 8])
        translate(v = [-1 * offset, 13, 0])
            rotate(a = [90, 0, -90])
                linear_extrude(height = 2, center = true, convexity = 10)
                    scale(v = [0.12, 0.12, 0.12])
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
    // To verify the mounting holes:
    /*
    translate(v = [0, 0, WALL_WIDTH])
        translate(v = [0, -20, 0])
            cylinder(h = SOLENOID_SCREW_HOLE_OFFSET_Y1, r = 10);
    translate(v = [0, 0, WALL_WIDTH])
        translate(v = [0, -20, 0])
            cylinder(h = SOLENOID_SCREW_HOLE_OFFSET_Y2, r = 10);
    */
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
