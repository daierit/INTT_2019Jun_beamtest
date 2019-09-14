// Analyze FVTX PRDF file and produce a root file containing the strip hit info
// Jan 2011, Jin Huang <jhuang@bnl.gov>

void Fun4FVTX_RecoPRDF(int run_number       =89, 
           const char* type = "junk", 
		       int nEvents          =0, 
		       int event_frame_check=0)
{
  // load libraries
  gSystem->Load("libfun4all");
  // gSystem->Load("offline/packages/fvtxoo/modules/lib/libfvtxoo_modules.so");
  gSystem->Load("libfun4allfuncs_muons");
  gSystem->Load("liblvl2");

  gSystem->Load("libfvtx_subsysreco");
  gSystem->Load("libfvtxgeom");
  FvtxGeom::set_public_file_path("./");

  cerr << "libraries loaded.\n";
  char input[255],output[255];
  //sprintf(input,"/sphenix/data/data03/phnxreco/sphenix/t1439/beam/beam-%06d-0000.prdf",run_number);
  //sprintf(output,"/sphenix/data/data03/phnxreco/sphenix/t1439/beam/data-%06d.root",run_number);
  //sprintf(input,"/sphenix/data/data02/sphenix/t1439/beam/beam-%06d-0000.prdf", run_number);
  sprintf(input,"/sphenix/data/data02/sphenix/t1439/2019/prdf/%s/%s-%08d-0000.prdf",type,type, run_number);
  //sprintf(input,"/sphenix/data/data02/sphenix/t1439/beam/beam-%06d-0000.prdf", run_number);
  //sprintf(input,"/sphenix/data/data02/sphenix/t1439/junk/junk_%08d-0000.prdf", run_number);
  //sprintf(input,"/sphenix/data/data02/sphenix/t1439/2019/prdf/macro/noise_%08d-0000.prdf", run_number);
  sprintf(output,"./data-%08d.root",run_number);
  std::cout<<"Input : "<<input <<std::endl
	   <<"Output: "<<output<<std::endl;

  ///////////////////////////////////////////
  // recoConsts setup
  //////////////////////////////////////////
  recoConsts *rc=recoConsts::instance();
  rc->set_IntFlag("PRINT_MUTOO_PARAMETERS",1);
  rc->set_CharFlag("SAVE_FVTX_UNPACK_ONLINE_DEBUG_TREE",output);
  rc->set_IntFlag("FVTX_UNPACK_EVENTFRAME_CHECK_MODE",event_frame_check);

  ///////////////////////////////////////////
  // Make the Server
  //////////////////////////////////////////
  Fun4AllServer *se=Fun4AllServer::instance();
  se->Verbosity(0);

  ///////////////////////////////////////////
  // Subsystems
  //////////////////////////////////////////
  se->registerSubsystem(new VtxReco());
  FvtxUnpackPRDF *Fvtx_prdf=new FvtxUnpackPRDF();
  se->registerSubsystem(Fvtx_prdf);

  ///////////////////////////////////////////
  // Analyze the Data.
  //////////////////////////////////////////
  Fun4AllInputManager *in=new Fun4AllPrdfInputManager("PRDFin");
  in->fileopen(input);
  se->registerInputManager(in);

  std::cout << "(Fun4AllServer *)se->run(" << nEvents << ");" << std::endl;
  se->run(nEvents);
  se->End();

  cout << "Completed reconstruction." << endl;

}
