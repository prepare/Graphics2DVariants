
/////////////////////////////////////////////////////////////////////
// LogFileManager.h -- Header file for the LogFileManager class
//
// Author       : Chad M. Draper
// Date         : February 16, 2007
// Description  : Manages log files
//
/////////////////////////////////////////////////////////////////////
// Update Log
//      Date            Description
//      ----            -----------
//      2007-02-16      Initial implementation
//

#ifndef LogFileManager_H_
#define LogFileManager_H_

#include "LogFile.h"
#include <map>
#include <sstream>

namespace cmd
{
    /** @brief Manages log files for an application
    
        An application will typically create a log file name <AppName>.log.
        However, if logging is to occur within libraries used by the application
        (and written to <AppName>.log, it is necessary to have a way to retrieve
        the appropriate log file without knowing the name.  The log file manager
        is a singleton, and there is only one manager per application instance.
        The manager should be used to create all log files, and will allow the
        application to set the default log file.  The manager can handle
        multiple log files, which can be retrieved by name, but the default file
        can be retrieved anywhere.
    */    
    class LogFileManager
    {
    private:
        static LogFileManager  mInstance; // One and only instance of the manager
        LogFileManager()                // Prevents multiple instances of the manager
        {
            mDefaultLog = 0;
        }
    
        typedef std::map< std::string, LogFile*, std::less< std::string > > LogList;
        LogList mLogs;                  // All logs maintained by the manager
        LogFile* mDefaultLog;           // Pointer to the default log file

    public:

        /** Get the manager instance */
        static LogFileManager& getInstance()
        {
            return mInstance;
        }
        /** Get a pointer to the manager instance */
        static LogFileManager* getInstancePtr()
        {
            return &mInstance;
        }
        
        /** Destructor */
        ~LogFileManager()
        {
            // Delete the log files
            LogList::iterator iter;
            for ( iter = mLogs.begin(); iter != mLogs.end(); iter++ )
            {
                delete iter->second;
            }
            mLogs.clear();
            mDefaultLog = 0;
        }

        /** Create a new log file
            @param  name            Name of the log file, also used as an index in the log list
            @param  isDefault       Indicates if this log file is the default log file
            @param  mode        Indicates the mode in which the file is opened
            @param  stream      If null, the output is not redirected.  Otherwise,
                                this is the stream to which output is directed.
        */
        LogFile* CreateLog( const std::string& name,
                            bool isDefault = false,
                            LogFile::LoggingLevel logLevel = LogFile::LoggingLevel::Normal,
                            std::ios_base::openmode mode = std::ios::out,
                            std::ostream* stream = 0 )
        {
            // See if the log file has already been created
            LogList::iterator iter = mLogs.find( name );
            if ( iter == mLogs.end() )
            {
                // Create the log file:
                LogFile* log = new LogFile( name, logLevel, mode, stream );
                
                // If this is the first log file, or it is marked as default, set
                // the default log file pointer:
                if ( !mDefaultLog || isDefault )
                    mDefaultLog = log;
                
                // Add the log to the list:
                mLogs.insert( LogList::value_type( name, log ) );
            
                // Return the new log file:
                return log;
            }
            else
                return iter->second;
        }
        
        /** Get a pointer to a specified log file */
        LogFile* GetLog( const std::string& name )
        {
            // Look through the list.  If the named log file is found, return
            // it's pointer.  If the file is not found, return null.
            LogList::iterator iter = mLogs.find( name );
            if ( iter != mLogs.end() )
                return iter->second;
            return 0;
        }
        
        /** Get the default log file */
        LogFile* GetDefaultLog() const
        {
            return mDefaultLog;
        }
        
        /** Set the default log file */
        void SetDefaultLog( LogFile* log )
        {
            mDefaultLog = log;
        }
        
        /** Set the logging level of the default log file */
        void SetLoggingLevel( LogFile::LoggingLevel lvl )
        {
            if ( mDefaultLog )
                mDefaultLog->SetLoggingLevel( lvl );
        }

        /** Get the lead-in string for the default log file */
        std::string lead_in( LogFile::ErrorCode errCode = LogFile::ErrorCode::Remark )
        {
            if ( GetDefaultLog() )
                return GetDefaultLog()->lead_in( errCode );
            return "";
        }

        /** Write a log message to the default log file */
        void LogMessage( const std::string& msg, LogFile::ErrorCode errCode = LogFile::ErrorCode::Remark, bool appendNewline = true )
        {
            if ( GetDefaultLog() )
                GetDefaultLog()->LogMessage( msg, errCode, appendNewline );
        }
        
        /** Push a new section onto the stack */
        void PushSection( const std::string& name )
        {
            if ( GetDefaultLog() )
                GetDefaultLog()->PushSection( name );
        }
        
        /** Pop a section from the stack */
        void PopSection()
        {
            if ( GetDefaultLog() )
                GetDefaultLog()->PopSection();
        }
        
        /** Allow remarks to be posted using the '<<' operator
            @remark Whatever is passed to this command WILL be written, and
                    labeled as a remark.
        */
        template< typename T >
        inline std::ostream& operator<<( const T& val )
        {
            if ( GetDefaultLog() )
            {
                return GetDefaultLog()->operator<<( val );
            }
            return std::cerr;
        }
        
    }; // class LogFileManager
    
    // Instantiate the instance
//    LogFileManager LogFileManager::mInstance;
    
} // namespace cmd

#endif  // LogFileManager_H_
