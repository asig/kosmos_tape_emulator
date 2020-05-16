/*
 * Copyright (c) 2018 Andreas Signer <asigner@gmail.com>
 *
 * This file is part of kosmos_tape_emulator.
 *
 * kosmos_tape_emulator is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * kosmos_tape_emulator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with kosmos_tape_tool.  If not, see <http://www.gnu.org/licenses/>.
 */

W = 100;
D = 85+65;
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

SD_CARD_W = 27;
SD_CARD_D = 45;
SD_CARD_H = 5;

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
    cube([SD_CARD_W, SD_CARD_D ,SD_CARD_H]);
}
    
module box() {
    translate([0,0,H/2])
    difference() {
        cube([W,D,H],true);
        translate([0,0,WALL_WIDTH]) cube([INNER_W,INNER_D,H],true);
    }
}

SUPPORT_R = 10;
SUPPORT_H1 = 5;
SUPPORT_H2 = 20;

module support_cylinder() {
    // Cylinder
    translate([0,0,-(SUPPORT_H1+SUPPORT_H2)]) cylinder(h=SUPPORT_H2, r1 = 0, r2 = SUPPORT_R, $fn=FRAGMENTS);
    translate([0,0,-SUPPORT_H1]) cylinder(h=SUPPORT_H1, r = SUPPORT_R, $fn=FRAGMENTS);
}

module support() {
    translate([0,0,H-FLOOR_HEIGHT]) difference() {
        support_cylinder();
        difference() {
            translate([-2*SUPPORT_R/2,-2*SUPPORT_R/2,-(SUPPORT_H1+SUPPORT_H2)]) cube([2*SUPPORT_R,2*SUPPORT_R,SUPPORT_H1+SUPPORT_H2]);
            translate([0,0,-(SUPPORT_H1+SUPPORT_H2)]) cube([2*SUPPORT_R/2,2*SUPPORT_R/2,SUPPORT_H1+SUPPORT_H2]);
        }
    }            
}

module support180() {
    translate([0,0,H-FLOOR_HEIGHT]) difference() {
        // Cylinder
        support_cylinder();
        translate([-2*SUPPORT_R/2,-2*SUPPORT_R/2,-(SUPPORT_H1+SUPPORT_H2)]) cube([2*SUPPORT_R,SUPPORT_R,SUPPORT_H1+SUPPORT_H2]);
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
        //translate([-W/2+SCREW_HOLE_OFFSET, -D/2+SCREW_HOLE_OFFSET,0]) cylinder(r=.751,h=WALL_WIDTH,center=true, $fn=100);
        //translate([-W/2+SCREW_HOLE_OFFSET,  0-SCREW_HOLE_OFFSET/2,0]) cylinder(r=.75,h=WALL_WIDTH,center=true, $fn=100);
        //translate([-W/2+SCREW_HOLE_OFFSET,  D/2-SCREW_HOLE_OFFSET,0]) cylinder(r=.75,h=WALL_WIDTH,center=true, $fn=100);
        //translate([ W/2-SCREW_HOLE_OFFSET, -D/2+SCREW_HOLE_OFFSET,0]) cylinder(r=.75,h=WALL_WIDTH,center=true, $fn=100);
        //translate([ W/2-SCREW_HOLE_OFFSET,  0-SCREW_HOLE_OFFSET/2,0]) cylinder(r=.75,h=WALL_WIDTH,center=true, $fn=100);
        //translate([ W/2-SCREW_HOLE_OFFSET,  D/2-SCREW_HOLE_OFFSET,0]) cylinder(r=.75,h=WALL_WIDTH,center=true, $fn=100);
    }
}
    
module case() {
    difference() {
        group() {
            box();
            translate([W/2-7,-7-65/2, 0]) led();
            translate([0,-18-65/2,0]) display_support();
            
            translate([-W/2+WALL_WIDTH, -D/2 + WALL_WIDTH, 0]) support();
            translate([-W/2+WALL_WIDTH,  0 - WALL_WIDTH, 0]) rotate([0,0,270]) support180();
            translate([-W/2+WALL_WIDTH,  D/2 - WALL_WIDTH, 0]) rotate([0,0,270]) support();
            translate([ W/2-WALL_WIDTH, -D/2 + WALL_WIDTH, 0]) rotate([0,0,90]) support();
            translate([ W/2-WALL_WIDTH,  0 - WALL_WIDTH, 0]) rotate([0,0,90]) support180();
            translate([ W/2-WALL_WIDTH,  D/2 - WALL_WIDTH, 0]) rotate([0,0,180]) support();

        };
    
        translate([W/2-7,-7-65/2,0]) led_hole();
        translate([0,20-65/2,0]) button_holes();            
        translate([0,-18-65/2,0]) display_hole();
        translate([INNER_W/2-SD_CARD_W,INNER_D/2-SD_CARD_D+WALL_WIDTH,1]) sd_hole();            
        translate([0,-D/2,H]) cable_hole();
        translate([-INNER_W/2+SCREW_HOLE_OFFSET, -INNER_D/2+SCREW_HOLE_OFFSET,H-WALL_WIDTH]) cylinder(r=.5,h=WALL_WIDTH,center=true);
        translate([-INNER_W/2+SCREW_HOLE_OFFSET,  0-SCREW_HOLE_OFFSET/2,H-WALL_WIDTH]) cylinder(r=.5,h=WALL_WIDTH,center=true);
        translate([-INNER_W/2+SCREW_HOLE_OFFSET,  INNER_D/2-SCREW_HOLE_OFFSET,H-WALL_WIDTH]) cylinder(r=.5,h=WALL_WIDTH,center=true);

        translate([ INNER_W/2-SCREW_HOLE_OFFSET, -INNER_D/2+SCREW_HOLE_OFFSET,H-WALL_WIDTH]) cylinder(r=.5,h=WALL_WIDTH,center=true);
        translate([ INNER_W/2-SCREW_HOLE_OFFSET,  0-SCREW_HOLE_OFFSET/2,H-WALL_WIDTH]) cylinder(r=.5,h=WALL_WIDTH,center=true);
        translate([ INNER_W/2-SCREW_HOLE_OFFSET,  INNER_D/2-SCREW_HOLE_OFFSET,H-WALL_WIDTH]) cylinder(r=.5,h=WALL_WIDTH,center=true);
    }
}


case();
translate([W+20,0,0]) 
ground_plate();

 