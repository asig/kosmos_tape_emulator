W = 100;
D = 85;
H = 50;

FRAGMENTS = 16;

WALL_WIDTH = 2;
FLOOR_HEIGHT = 2;

DISP_W = 72;
DISP_D = 25;

DISP_SUPPORT_WALL_W = 3;
DISP_SUPPORT_W = DISP_W + 2*DISP_SUPPORT_WALL_W;
DISP_SUPPORT_D = DISP_D + 2*DISP_SUPPORT_WALL_W;
DISP_SUPPORT_H = 7;

SD_CARD_D = 5;
SD_CARD_H = 45;

INNER_W = W - 2*WALL_WIDTH;
INNER_D = D - 2*WALL_WIDTH;

SCREW_HOLE_OFFSET = 4;

module button_holes() {
    R = 6.5;
    translate([-17,0,0]) cylinder(r=R, h=2*H, center = true);
    translate([17,0,0]) cylinder(r=R, h=2*H, center = true);
    translate([0,-9,0]) cylinder(r=R, h=2*H, center = true);
    translate([0,9,0]) cylinder(r=R, h=2*H, center = true);
}

module display_support() {    
    translate([-DISP_SUPPORT_W/2,-DISP_SUPPORT_D/2,0]) {
        difference() {
            cube([DISP_SUPPORT_W, DISP_SUPPORT_D, DISP_SUPPORT_H]);        
            translate([DISP_W + DISP_SUPPORT_WALL_W, DISP_SUPPORT_WALL_W, DISP_SUPPORT_H-4]) {
                cube([DISP_SUPPORT_WALL_W, DISP_D, 4]);
            }        
        }    
    }
}

module display_hole() {
    cube([DISP_W,DISP_D, 2*H],true);
}

module sd_hole() {
    W = 27;    
    cube([SD_CARD_D,W,SD_CARD_H]);
}
    
module box() {
    translate([0,0,H/2])
    difference() {
        cube([W,D,H],true);
        translate([0,0,WALL_WIDTH]) cube([INNER_W,INNER_D,H],true);
    }
}

module support() {
    R = 10;
    H1 = 5;
    H2 = 20;

    translate([0,0,H-FLOOR_HEIGHT]) difference() {
        // Cylinder
        group() {    
            translate([0,0,-(H1+H2)]) cylinder(h=H2, r1 = 0, r2 = R, $fn=FRAGMENTS);
            translate([0,0,-H1]) cylinder(h=H1, r = R, $fn=FRAGMENTS);
        }
        difference() {
            translate([-2*R/2,-2*R/2,-(H1+H2)]) cube([2*R,2*R,H1+H2]);
            translate([0,0,-(H1+H2)]) cube([2*R/2,2*R/2,H1+H2]);
        }
    }            
}

module led() {
    translate([0,0,1.5]) cylinder(h=3, r = 3, $fn=FRAGMENTS, center = true);
}

module led_hole() {
    translate([0,0,1.5]) cylinder(h=3, r = 1.5, $fn=FRAGMENTS, center = true);
}

module cable_hole() {
    rotate([180,0,0])
    translate([0,-WALL_WIDTH/2,WALL_WIDTH/2]) {
        translate([0,0,WALL_WIDTH/2]) rotate([90,0,0]) cylinder(r=2,h=4, $fn=100, center=true);
        cube([8,WALL_WIDTH+.1,WALL_WIDTH+.1], center=true);
    }
}

module ground_plate() {
    difference() {
        group() {
            SHRINK = 1;
            W = INNER_W-SHRINK;
            D = INNER_D-SHRINK;
            translate([0,-(D+WALL_WIDTH)/2,0]) cube([7,WALL_WIDTH+.1,WALL_WIDTH+.1], center=true);
            cube([W, D, WALL_WIDTH],center=true);
        }
        translate([-W/2+SCREW_HOLE_OFFSET, -D/2+SCREW_HOLE_OFFSET,0]) cylinder(r=.751,h=WALL_WIDTH,center=true, $fn=100);
        translate([W/2-SCREW_HOLE_OFFSET, -D/2+SCREW_HOLE_OFFSET,0]) cylinder(r=.75,h=WALL_WIDTH,center=true, $fn=100);
        translate([-W/2+SCREW_HOLE_OFFSET, D/2-SCREW_HOLE_OFFSET,0]) cylinder(r=.75,h=WALL_WIDTH,center=true, $fn=100);
        translate([W/2-SCREW_HOLE_OFFSET, D/2-SCREW_HOLE_OFFSET,0]) cylinder(r=.75,h=WALL_WIDTH,center=true, $fn=100);
    }
}
    
module case() {
    difference() {
        group() {
            box();
            translate([W/2-7,-7, 0]) led();
            translate([0,-18,0]) display_support();
            
            translate([-W/2+WALL_WIDTH, -D/2 + WALL_WIDTH, 0]) support();
            translate([W/2-WALL_WIDTH, -D/2 + WALL_WIDTH, 0]) rotate([0,0,90]) support();
            translate([W/2-WALL_WIDTH, D/2 - WALL_WIDTH, 0]) rotate([0,0,180]) support();
            translate([-W/2+WALL_WIDTH, D/2 - WALL_WIDTH, 0]) rotate([0,0,270]) support();

        };
    
        translate([W/2-7,-7,0]) led_hole();
        translate([0,20,0]) button_holes();            
        translate([0,-18,0]) display_hole();
        translate([-W/2+1,0,0]) sd_hole();            
        translate([0,-D/2,H]) cable_hole();
        translate([-INNER_W/2+SCREW_HOLE_OFFSET, -INNER_D/2+SCREW_HOLE_OFFSET,H-WALL_WIDTH]) cylinder(r=.5,h=WALL_WIDTH,center=true);
        translate([INNER_W/2-SCREW_HOLE_OFFSET, -INNER_D/2+SCREW_HOLE_OFFSET,H-WALL_WIDTH]) cylinder(r=.5,h=WALL_WIDTH,center=true);
        translate([-INNER_W/2+SCREW_HOLE_OFFSET, INNER_D/2-SCREW_HOLE_OFFSET,H-WALL_WIDTH]) cylinder(r=.5,h=WALL_WIDTH,center=true);
        translate([INNER_W/2-SCREW_HOLE_OFFSET, INNER_D/2-SCREW_HOLE_OFFSET,H-WALL_WIDTH]) cylinder(r=.5,h=WALL_WIDTH,center=true);
    }
}


case();

//translate([W+20,0,0]) 
ground_plate();

 