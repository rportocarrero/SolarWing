#include <iostream>
#include <random>
#include <fstream>

using namespace std;

// Limits
int WING_SPAN_MAX           = 2;        // (m)
int WING_SPAN_MIN           = 0;        // (m)
int ROOT_CHORD_MIN          = 0;        // (m) 
int ROOT_CHORD_MAX          = 1;        // (m)
int TAPER_RATIO_MIN         = 0;
int TAPER_RATIO_MAX         = 1;
float SWEEPBACK_ANGLE_MAX   = M_PI/2;   // (rad)
float SWEEPBACK_ANGLE_MIN   = 0;        // (rad)
float DIHEDRAL_ANGLE_MAX    = M_PI/2;   // (rad)
float DIHEDRAL_ANGLE_MIN    = 0;        // (rad)
float WASHOUT_ANGLE_MAX     = M_PI/2;   // (rad)
float WASHOUT_ANGLE_MIN     = -M_PI/2;  // (rad)
float ELEVON_CHORD_MIN      = 0;      // (chord)
float ELEVON_CHORD_MAX      = 1;      // (chord)
float ELEVON_SPAN_MAX       = 1;        // (span)
float ELEVON_SPAN_MIN       = 0.1;      // (span)
float ELEVON_OFFSET_MAX       = 0.5;      // (span)
float ELEVON_OFFSET_MIN       = 0;        // (span)

// Basic planform variables
struct Planform{
    float wingSpan;
    float rootChord;
    float tipChord;
    float taperRatio;
    float sweepbackAngle;
    float dihedralAngle;
    float washoutAngle;
    float elevonChord;
    float elevonSpan;
    float elevonOffset;
};

// Random number generator
float randGen(int min, int max){
    random_device rd;
    default_random_engine eng(rd());
    uniform_real_distribution<> distr(min, max);
    return distr(eng);
}

// Generate a new wing design
Planform generatePlanform(){
    Planform design;

    design.wingSpan         = randGen(WING_SPAN_MIN,WING_SPAN_MAX);
    design.rootChord        = randGen(ROOT_CHORD_MIN,ROOT_CHORD_MAX);
    design.taperRatio       = randGen(TAPER_RATIO_MIN, TAPER_RATIO_MAX);
    design.tipChord         = design.rootChord * design.taperRatio;
    design.sweepbackAngle   = randGen(SWEEPBACK_ANGLE_MIN, SWEEPBACK_ANGLE_MAX);
    design.dihedralAngle    = randGen(DIHEDRAL_ANGLE_MIN, DIHEDRAL_ANGLE_MAX);
    design.washoutAngle     = randGen(WASHOUT_ANGLE_MIN,WASHOUT_ANGLE_MAX);
    design.elevonChord      = randGen(ELEVON_CHORD_MIN, ELEVON_CHORD_MAX);
    design.elevonSpan       = design.wingSpan/2 * randGen(ELEVON_SPAN_MIN, ELEVON_SPAN_MAX);
    design.elevonOffset     = design.wingSpan/2 * randGen(ELEVON_OFFSET_MIN, ELEVON_OFFSET_MAX);

    return design;
}

void printPlanform(Planform design){
    cout << "Wing Span:\t" << design.wingSpan << " (m)\n";
    cout << "Root Chord:\t" << design.rootChord << " (m)\n";
    cout << "Tip Chord:\t" << design.tipChord << " (m)\n";
    cout << "Taper Ratio:\t" << design.taperRatio << " (m)\n";
    cout << "Sweepback Angle:\t" << design.sweepbackAngle << " (rad)\n";
    cout << "Dihedral Angle:\t" << design.dihedralAngle << " (rad)\n";
    cout << "Washout Angle:\t" << design.washoutAngle << " (rad)\n";
    cout << "Elevon Span:\t" << design.elevonSpan << " (span/2)\n";
    cout << "Elevon Chord:\t" << design.elevonChord << " (chord))\n";
    cout << "Elevon Offset:\t" << design.elevonOffset << " (span/2)\n";

}

void writeSVG(Planform design, int n){
    int svgWidth = 1100;
    int svgHeight = 850;
    int scaleFactor = 1000;

    int half_span = design.wingSpan * (scaleFactor);
    int root_TE = design.rootChord * (scaleFactor);
    int tip_LE = sin(design.dihedralAngle) * half_span;
    int tip_TE = tip_LE + (design.tipChord*scaleFactor);

    // Deriving the elevon hinge line
    float x1 = 0;   // centerline
    float x2 = half_span;   // wing tip
    float y1 = (1-design.elevonChord) * design.rootChord;  // centerline intercept of elevon hinge line
    float y2 = (1-design.elevonChord) * design.tipChord + tip_LE;   // wingtip intercept of elevon hinge line

    float det = x1-x2;
    float m = (y1 / det) - (y2 / det);
    float b = ((-y1*x2) / det) + ((x1*y2) / det);   //y = mx + b equation of elevon hinge line

    float elevon_root_LE_X = (half_span-(design.elevonSpan+design.elevonOffset)*scaleFactor);
    float elevon_tip_LE_X = half_span-(design.elevonOffset)*scaleFactor;
    float elevon_root_LE_Y = m * elevon_root_LE_X + b;
    float elevon_tip_LE_Y = m * elevon_tip_LE_X + b;
    // Elevon coordinates
    

    ofstream outfile;
    string fileName = to_string(n)+".svg";
    outfile.open(fileName);
    outfile << "<?xml version=\"1.0\" standalone=\"no\"?>\n";
    outfile << "<svg width = \""<< svgWidth << "\" height = \"" << svgHeight << "\" xmlns=\"http://www.w3.org/2000/svg\">\n";

    // Dimension box

    // Top View Box

    // Front View Box

    // Side View Box

    // Draw wing planform
    outfile <<  "<polygon points=\"0,0 " << half_span << "," << tip_LE << " " << half_span << "," << tip_TE << " 0," << root_TE << "\"  style=\"fill:white;stroke:black;stroke-width:1\"/>\n";
    
    // Draw elevons
    
    outfile << "</svg>";
    outfile.close();
}

int main(){

    for(int i=0; i<50; i++){
        Planform newDesign = generatePlanform();
        printPlanform(newDesign);
        writeSVG(newDesign,i);
    }
    
    return 0;
}