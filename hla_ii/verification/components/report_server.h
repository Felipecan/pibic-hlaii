#ifndef REPORT_CLASS
  #define REPORT_CLASS

  //+--------------------------------------------------------------------------
  //| Typedef enum
  //+--------------------------------------------------------------------------
  typedef enum severity {INFO_MSG, ERROR_MSG} severity; // Define type of msg

  typedef enum verbosity_type {LOW, MEDIUM, HIGH} verbosity_type; // Define verbosity

  
  //+--------------------------------------------------------------------------
  //| NAMESPACE: report_server
  //+--------------------------------------------------------------------------
  namespace report_server {

  //+--------------------------------------------------------------------------
  //| CLASS: report
  //+--------------------------------------------------------------------------
  //! Classe responsible to print msg on log files and at screen.
  //+--------------------------------------------------------------------------
    class report{
     protected :
    //+--------------------------------------------------------------------------
    //| Data Members
    //+--------------------------------------------------------------------------
       static ofstream       file;         // Log file
       static bool           quiet;        // If True no msg is print on screen
       static verbosity_type verbosity;    // Default of simulation vebosity
       static int            total_errors; // Errors couter
  
     public :  
     //+--------------------------------------------------------------------------
     //| FUNCTION: set_log_file_name
     //+--------------------------------------------------------------------------
     //! Create a log file based in a const char* input variable
     //+--------------------------------------------------------------------------
       static void set_log_file_name(const char* fname){       
	 time_t rawtime;
         struct tm * timeinfo;

         time (&rawtime);
         timeinfo = localtime (&rawtime);
          file.open(fname);
	  file << "\t\tSC VERIFICATION" << endl;
          file << "\t\t" << asctime(timeinfo) << endl;
       }

     //+--------------------------------------------------------------------------
     //| FUNCTION: set_verbosity
     //+--------------------------------------------------------------------------
     //! Setup the server verbosity and if the server will be in the quiet mode
     //!
     //! Verbosity definition:
     //!   - LOW : low verbosity messages it will be printed. 
     //!   - MEDIUM : it will be printed low and medium verbosity messages.
     //!   - HIGH : all messages will be prited.
     //+--------------------------------------------------------------------------
       static void set_verbosity(verbosity_type verbosity_input, bool q){
           verbosity = verbosity_input;
           quiet = q;
       }

     //+--------------------------------------------------------------------------
     //| FUNCTION: compose_msg
     //+--------------------------------------------------------------------------
     //! Function reseponsible to compose the mensage. 
     //! 
     //! Compose a message based on type severity (INFO, ERROR),  and increment 
     //! total error counter if is a error message.
     //+--------------------------------------------------------------------------
       static string compose_msg(const char* id, const char* msg, severity s) {
          stringstream out;
  
          if(s == INFO_MSG) {
             out << "INFO@";
  	}
  	else {
  	   out << "ERROR@";
	   total_errors++;
  	}
  
    	out<< sc_core::sc_time_stamp() << ": [" << id << "] " << msg << std::endl;
  
  	return out.str();
       }
  
     //+--------------------------------------------------------------------------
     //| FUNCTION: msg_save
     //+--------------------------------------------------------------------------
     //! Print the message generate by compose_msg() in the log file
     //!
     //! @see compose_msg()
     //+--------------------------------------------------------------------------
       static void msg_save(string msg){
            file << msg;
       }
  
     //+--------------------------------------------------------------------------
     //| FUNCTION: server
     //+--------------------------------------------------------------------------
     //! Call the compose_msg() function to compose the message, print the message
     //! base in the verbosity and quiet mode. All message are save in a file log 
     //! based only in the verbosity.
     //!
     //! @see compose_msg()
     //! @see msg_save()
     //+--------------------------------------------------------------------------
       static void server(const char* id, const char* msg, severity s, verbosity_type v){       
         string comp_msg;
  	 comp_msg = compose_msg(id, msg, s);
  
         if(!quiet){         
	   if(v <= verbosity) {
	     std::cout << comp_msg;
	   }
         }           
         if(v <= verbosity) {
  	   msg_save(comp_msg);
         }
       }

     //+--------------------------------------------------------------------------
     //| FUNCTION: get_errors
     //+--------------------------------------------------------------------------
     //! Return how many times an error message was call
     //+--------------------------------------------------------------------------
       static int get_errors(){
	 return total_errors;
       }
    };

    //+--------------------------------------------------------------------------
    //| FUNCTION: INFO
    //+--------------------------------------------------------------------------
    //! Call the server for a information message. The verbosity type is option, 
    //! default is HIGH.
    //+--------------------------------------------------------------------------
    static void INFO(const char* id, const char* msg, verbosity_type v = HIGH){
      report::server( id, msg, INFO_MSG, v);
    }

    //+--------------------------------------------------------------------------
    //| FUNCTION: ERROR
    //+--------------------------------------------------------------------------
    //! Call the server for a error message. The verbosity type is option, 
    //! default is LOW.
    //+--------------------------------------------------------------------------
    static void ERROR(const char* id, const char* msg, verbosity_type v = LOW){
      report::server( id, msg, ERROR_MSG, v);
    }

  }

  //+--------------------------------------------------------------------------
  //| Declare static class data members
  //+--------------------------------------------------------------------------
  ofstream  report_server::report::file;
  bool      report_server::report::quiet;
  int       report_server::report::total_errors;
  

  //+--------------------------------------------------------------------------
  //| Using namespace
  //+--------------------------------------------------------------------------
  using namespace report_server;
#endif

