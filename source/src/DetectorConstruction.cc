/// \file DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"
#include "G4RotationMatrix.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
//#include "aPixel.hh"
// For the sensitive volume
//#include "G4SDManager.hh"
//#include "SensitiveVolume.hh"
#include "DetectorConstruction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal 
G4GlobalMagFieldMessenger* DetectorConstruction::fMagFieldMessenger = nullptr; 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction()
{
  // Parameter setting ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  // XY setting
  G4int NX    = 128;
  G4int NY    = NX;
  G4int DUTNX = 1000;
  G4int DUTNY = DUTNX;
  G4double Wx = 8.0;
  G4double Wy = Wx;
  
  // Z setting
  G4int t_depl      = 30;
  G4int t_wafer     = 10;
  G4int t_depl_DUT  = 300;
  G4int t_wafer_DUT = 10;
  G4int t_cmos      = 10;
  // other setting
  G4int NN  = 2;
  G4int sig =2.0;  
  
  // making aPixel ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  //                    ID  NX    NY    NN   Wx      Wy      sig      t(cmos)     t(depl)        t(wafer)
  fpix0    = new aPixel(10, NX,   NY,   NN,  Wx*um,  Wy*um,  sig*um,  t_cmos*um,  t_depl*um,     t_wafer*um);     //  Tracker0
  fpix1    = new aPixel(20, NX,   NY,   NN,  Wx*um,  Wy*um,  sig*um,  t_cmos*um,  t_depl*um,     t_wafer*um);     //  Tracker1
  fpix2    = new aPixel(30, NX,   NY,   NN,  Wx*um,  Wy*um,  sig*um,  t_cmos*um,  t_depl*um,     t_wafer*um);     //  Tracker2
  sofist0  = new aPixel(40, DUTNX,DUTNY,NN,  1.0*um, 1.0*um, sig*um,  t_cmos*um,  t_depl_DUT*um, t_wafer_DUT*um); //  DUT
  
  sofist1  = new aPixel(50, NX,  NY,  NN,  Wx*um,  Wy*um,  sig*um,  t_cmos*um,  t_depl*um,     t_wafer*um);     //  aftre DUT (unused)
  fpix3    = new aPixel(60, NX,  NY,  NN,  Wx*um,  Wy*um,  sig*um,  t_cmos*um,  t_depl*um,     t_wafer*um);     //  aftre DUT (unused)
  
  /*  // backup of prot type setting
  fpix0    = new aPixel(10, 128,  128,  2,  8.0*um,  8.0*um,  3*um, 10*um, 50*um, 440*um);
  fpix1    = new aPixel(20, 128,  128,  2,  8.0*um,  8.0*um,  3*um, 10*um, 50*um, 440*um);
  sofist0  = new aPixel(30,  50,   50,  2, 20.0*um, 20.0*um,  3*um, 10*um, 50*um, 440*um);
  sofist1  = new aPixel(40,  50,   50,  2, 20.0*um, 20.0*um,  3*um, 10*um, 50*um, 440*um);
  fpix2    = new aPixel(50, 128,  128,  2,  8.0*um,  8.0*um,  3*um, 10*um, 50*um, 440*um);
  fpix3    = new aPixel(60, 128,  128,  2,  8.0*um,  8.0*um,  3*um, 10*um, 50*um, 440*um);
  */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......




DetectorConstruction::~DetectorConstruction()
{ 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
G4VPhysicalVolume* DetectorConstruction::Construct()
{
// Define materials 
DefineMaterials();

// Define volumes
return DefineVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials()
{ 
// Lead material defined using NIST Manager
//auto nistManager = G4NistManager::Instance();
//nistManager->FindOrBuildMaterial("G4_Pb");

G4double a;  // mass of a mole;
G4double z;  // z=mean number of protons;  
G4double density; 
G4String symbol;
G4int ncomponents, natoms;
//new G4Material("liquidArgon", z=18., a= 39.95*g/mole, density= 1.390*g/cm3);
     // The argon by NIST Manager is a gas with a different density
new G4Material("Iron", z=26., a= 56*g/mole, density= 7.9*g/cm3);

// Vacuum
new G4Material("Galactic", z=1., a=1.01*g/mole,density= universe_mean_density,
	      kStateGas, 2.73*kelvin, 3.e-18*pascal);

// new G4Material("Aluminium", z=13., a=26.98*g/mole, density=2.700*g/cm3);
// new G4Material("Nitrogen", z=7.0,  a=14*g/mole, density=0.1*g/cm3);

 G4Element* Alu = new G4Element("Aluminum",symbol="Al" , z= 13., a= 26.9*g/mole);
 G4Element* Nit = new G4Element("Nitrogen",symbol="N" , z= 7., a= 14.0*g/mole);
 G4Element* Oxy = new G4Element("Oxygen",symbol="O" , z= 8., a= 16*g/mole);
 G4Material* AlN = new G4Material("AlN",density= 3.200*g/cm3, ncomponents=2);
 AlN->AddElement(Alu, natoms=1);
 AlN->AddElement(Nit , natoms=1);
 G4Material* Al2O3 = new G4Material("Al2O3",density= 3.800*g/cm3, ncomponents=2);
 Al2O3->AddElement(Alu, natoms=2);
 Al2O3->AddElement(Oxy , natoms=3);


// Print materials
G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::DefineVolumes()
{
G4cout << "DETCON start" << G4endl;
 
//Postion setting
G4double tr_pitch = 30.0;
G4double DUT_z    = 30.0;
 
// Get pointer to 'Material Manager'
G4NistManager* materi_Man = G4NistManager::Instance();

// Define 'World Volume'
// Define the shape of solid
G4double leng_X_World = 0.2*m;         // X-full-length of world
G4double leng_Y_World = 0.2*m;         // Y-full-length of world
G4double leng_Z_World = 0.5*m;         // Z-full-length of world
G4Box* solid_World =
 new G4Box("Solid_World", leng_X_World/2.0, leng_Y_World/2.0, leng_Z_World/2.0);

// Define logical volume
G4Material* materi_World = materi_Man->FindOrBuildMaterial("G4_AIR");
G4LogicalVolume* logVol_World =
 new G4LogicalVolume(solid_World, materi_World, "LogVol_World");
//logVol_World->SetVisAttributes (G4VisAttributes::Invisible);

// Placement of logical volume
G4int copyNum_World = 0;               // Set ID number of world
G4PVPlacement* physVol_World  =
 new G4PVPlacement(G4Transform3D(), "PhysVol_World", logVol_World, 0, false, copyNum_World);

// Prepare pixel sensors
 G4LogicalVolume *lV_fpix0  = fpix0   ->Getlogvol();
 G4LogicalVolume *lV_fpix1  = fpix1   ->Getlogvol();
 G4LogicalVolume *lV_fpix2  = fpix2   ->Getlogvol();
 G4LogicalVolume *lV_fpix3  = fpix3   ->Getlogvol();
 G4LogicalVolume *lV_sofist0= sofist0 ->Getlogvol();
 G4LogicalVolume *lV_sofist1= sofist1 ->Getlogvol();

//   G4cout << "DETCON lv0/lv1=" << lV_Pixel0 << "/" <<lV_Pixel1 << G4endl;



// Placement of the pixels to the world: Put the 'global envelop'

 G4double pos_X, pos_Y, pos_Z;
 G4ThreeVector vec;
 G4RotationMatrix rot;
 G4Transform3D trans;
 G4int cN;

//Tracker0 
pos_X = 0.000*mm;
pos_Y = 0.000*mm;
pos_Z = 0.0*mm;
vec = G4ThreeVector(pos_X, pos_Y, pos_Z);
rot = G4RotationMatrix();
trans = G4Transform3D(rot, vec);
cN = 1000;
new G4PVPlacement(trans, "PV_fpix0", lV_fpix0, physVol_World,false, cN);

//Tracker1
pos_X = 0.0*cm;
pos_Y = 0.0*mm;
pos_Z += tr_pitch*mm;
vec = G4ThreeVector(pos_X, pos_Y, pos_Z);
rot =  G4RotationMatrix();
//rot.rotateZ(0.4*deg);
trans = G4Transform3D(rot, vec);
cN=cN+10;
new G4PVPlacement(trans, "PV_fpix1", lV_fpix1, physVol_World,false, cN);

//Tracker2 
pos_X = 0.0*cm;
pos_Y = 0.0*cm;
pos_Z += tr_pitch*mm;
vec = G4ThreeVector(pos_X, pos_Y, pos_Z);
rot = G4RotationMatrix();
//rot.rotateY(1*deg);
trans = G4Transform3D(rot, vec);
cN=cN+10;
new G4PVPlacement(trans, "PV_fpix2", lV_fpix2, physVol_World,false, cN);
 
//DUT 
pos_X = 0.0*cm;
pos_Y = 0.0*cm;
//pos_Z = 90.0*mm;
pos_Z += DUT_z*mm;
vec = G4ThreeVector(pos_X, pos_Y, pos_Z);
rot = G4RotationMatrix();
//rot.rotateZ(0.5*deg);
trans = G4Transform3D(rot, vec);
cN=cN+10;
new G4PVPlacement(trans, "PV_sofist0", lV_sofist0, physVol_World,false, cN);

 
// Kyocera LSI package package:  Default material is Alumina ( A440 or A445 )
G4Box* CeraPKG= new G4Box("CeraPKG", 5.0*mm , 5.0*mm, 1.27*mm/2.0);
//  G4Box* CeraPKG= new G4Box("CeraPKG", 1.0*mm , 1.0*mm, 1.27*mm/2.0);  //backup
#if   0
G4LogicalVolume* lV_CeraPKG = new G4LogicalVolume(CeraPKG,materi_World, "CeraPKG");
#elif 0
G4LogicalVolume* lV_CeraPKG = new G4LogicalVolume(CeraPKG,materi_Man->FindOrBuildMaterial("AlN"), "CeraPKG");
#elif 0
G4LogicalVolume* lV_CeraPKG = new G4LogicalVolume(CeraPKG,materi_Man->FindOrBuildMaterial("Iron"), "CeraPKG");
#else
G4LogicalVolume* lV_CeraPKG = new G4LogicalVolume(CeraPKG,materi_Man->FindOrBuildMaterial("Al2O3"), "CeraPKG");
#endif
pos_X = 0.0*cm;
pos_Y = 0.0*cm;
pos_Z += 3.0*mm;
vec = G4ThreeVector(pos_X, pos_Y, pos_Z);
rot = G4RotationMatrix();
trans = G4Transform3D(rot, vec);
cN=cN+10;
// new G4PVPlacement(trans, "PV_CeraPKG", lV_CeraPKG, physVol_World,false, cN);


// atfer DUT1 
pos_X =  -0.00*mm;
pos_Y =  0.0*mm;
pos_Z = tr_pitch*mm;
vec = G4ThreeVector(pos_X, pos_Y, pos_Z);
rot =  G4RotationMatrix();
//rot.rotateZ(-1.2*deg);
trans = G4Transform3D(rot, vec);
cN=cN+10;
//new G4PVPlacement(trans, "PV_sofist1", lV_sofist1, physVol_World,false, cN);
 
//after DUT2 
pos_X =  0.0*mm;
pos_Y =  0.0*mm;
pos_Z = tr_pitch*mm;
vec = G4ThreeVector(pos_X, pos_Y, pos_Z);
rot =  G4RotationMatrix();
//rot.rotateX(45.*deg);
trans = G4Transform3D(rot, vec);
cN=cN+10;
//new G4PVPlacement(trans, "PV_fpix3", lV_fpix3, physVol_World,false, cN);

#if 0
// Sensitive Volume

// The name is arbitrary
auto aCmos= new SensitiveVolume("Cmos_SV");
// Add sensitivity to the logical volume
logVol_PixCmos -> SetSensitiveDetector(aCmos);
//register to the SensitiveDetectorManager
   auto SDman= G4SDManager::GetSDMpointer();
   SDman->AddNewDetector(aCmos);  

// The name is arbitrary
   auto aDepl= new SensitiveVolume("Depl_SV");
// Add sensitivity to the logical volume
   logVol_PixDepl  -> SetSensitiveDetector(aDepl);
//register to the SensitiveDetectorManager
//   auto SDman= G4SDManager::GetSDMpointer();
   SDman->AddNewDetector(aDepl);  

// The name is arbitrary
   auto aWafer= new SensitiveVolume("Wafer_SV");
// Add sensitivity to the logical volume
   logVol_PixWafer  -> SetSensitiveDetector(aWafer);
//register to the SensitiveDetectorManager
//   auto SDman= G4SDManager::GetSDMpointer();
   SDman->AddNewDetector(aWafer);  
#endif

   
   
// Return the physical world
//   logVol_World->SetVisAttributes (G4VisAttributes::GetInvisible());

//  auto simpleBoxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
//  simpleBoxVisAtt->SetVisibility(true);
//  calorLV->SetVisAttributes(simpleBoxVisAtt);

  //
  // Always return the physical World
  //
   return physVol_World;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructSDandField()
{ 
  // Create global magnetic field messenger.
  // Uniform magnetic field is then created automatically if
  // the field value is not zero.
  G4ThreeVector fieldValue;
  fMagFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
  fMagFieldMessenger->SetVerboseLevel(1);
  
  // Register the field messenger for deleting
  G4AutoDelete::Register(fMagFieldMessenger);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
