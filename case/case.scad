use <BOSL/transforms.scad>
use <case_top.scad>
use <case_bottom.scad>
use <display_plate.scad>

// --- global configuration ---
// (actually do not mess with it)
w=71;
l=76;
f=5;

// --- render ---
zdistribute(25) {
    case_bottom(w, l, f);
    case_top(w, l, f);
    translate([0, 35, -10]) display_plate(f, "Roboto:style=Bold");
}