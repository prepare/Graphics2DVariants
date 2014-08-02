
/////////////////////////////////////////////////////////////////////
// LogFile.h -- Header file for LogFile class.
//
// AUTHOR       : Chad M. Draper
// DATE         : January 18, 2006
// DESCRIPTION  : Directs output to a specified stream, allowing logging
//                using std::cout.
//
/////////////////////////////////////////////////////////////////////
// UPDATE LOG
//      DATE        DESCRIPTION
//      ----        -----------
//      2006-01-18  Initial implementation
//

#ifndef LOGFILE_H_
#define LOGFILE_H_

#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <stack>
//#include <boost/format.hpp>
#include <time.h>

#include "StringUtil.h"


namespace cmd
{

    /** @class LogFile
        Provides a class for logging messages, which is also capable of
        redirecting output (such as to std::cout.)
    */
    // Create a class for redirection
    class LogFile
    {
    public:

        /** Error codes
            @remark These affect how the error messages are displayed
        */
        enum ErrorCode
            {
                Remark = 1,
                Warning,
                Error,
                SectionStart,
                SectionEnd
            };
        
        /** Logging level
            @remark Indicates how verbose the logging should be.  Basically,
                    anything that's an error or higher will always be written
                    (unless logging is disabled.)  Other comments will depend on
                    the current logging level.
        */
        enum LoggingLevel
            {
                Disabled = 0,       /**< All logging is disabled.  Not very useful... */
                Low,                /**< Only Error or higher is logged. */
                Normal,             /**< Warnings and higher are logged. */
                Everything,         /**< All messages are logged. */
                
                Threshold           /**< If the current logging level + the error
                                         code is >= Threshold, the message is logged. */
            };

    private:
        std::ofstream*  mLogFile;               /**< Output file stream */
        std::ostream*   mOutputStream;          /**< Pointer to the output stream (default is std::cout) */
        std::streambuf* mOrigStreamBuf;         /**< Pointer to the original stream */
        bool            mRedirectStream;        /**< Indicates if the logfile is a
                                                     redirection of a stream (i.e. std::cout) */

        LoggingLevel    mLogLevel;              /**< Current level of detail. */
        std::stack< std::string > mSections;    /**< Stack of the sections in the log file.  Each
                                                     section is indented one level. */

    public:

        /** Add a new section level */
        void PushSection( const std::string& label )
        {
            // Write the label
            LogMessage( label, ErrorCode::SectionStart );
            
            // Increment the section level
            mSections.push( label );
        }
        
        /** Remove a section level */
        void PopSection()
        {
            // Get the name of the top-most section
            if ( mSections.size() )
            {
                std::string label = mSections.top();

                // Remove the section
                mSections.pop();

                // Write the label:
                LogMessage( label, ErrorCode::SectionEnd );
            }
        }

    public:

        /** Constructor
            @param  logFile     Name of the log file
            @param  mode        Indicates the mode in which the file is opened
            @param  stream      If null, the output is not redirected.  Otherwise,
                                this is the stream to which output is directed.
        */
        LogFile( const std::string& logFile, LoggingLevel logLevel = LoggingLevel::Normal, std::ios_base::openmode mode = std::ios::out, std::ostream* stream = &std::cout )
            : mLogFile( 0 ),
              mOrigStreamBuf( 0 ),
              mOutputStream( 0 ),
              mRedirectStream( false ),
              mLogLevel( logLevel )
        {
            Reopen( logFile, mode, stream );
        }

        /** Deconstructor
            Output is restored to the original buffer if redirection was enabled.
        */        
        ~LogFile()
        {
            Close();
        }

        /** Set the logging level */
        void SetLoggingLevel( LoggingLevel lvl )
        {
            mLogLevel = ( lvl >= LoggingLevel::Threshold ) ? LoggingLevel::Threshold : lvl;
        }

        /** Close the current stream, and open an new one
            @param  logFile     Name of the log file
            @param  mode        Indicates the mode in which the file is opened
            @param  stream      If null, the output is not redirected.  Otherwise,
                                this is the stream to which output is directed.
        */
        void Reopen( const std::string& logFile, std::ios_base::openmode mode = std::ios::out, std::ostream* stream = &std::cout )
        {
            // Close the stream, assuming it is open:
            Close();

            // Make sure the file path exists:
            std::string path, filename;
            StringUtil::SplitFilename( logFile, &path, &filename );
#ifdef  BOOST_FILESYSTEM
            StringUtil::CreatePath( path );
#endif

            // Create the new log file and direct output to it:
            mLogFile = new std::ofstream;
            mLogFile->open( logFile.c_str(), mode );
            
            if ( stream )
            {
                mOrigStreamBuf = stream->rdbuf( mLogFile->rdbuf() );
                mOutputStream = stream;

                mRedirectStream = true;
            }
            else
            {
                mOrigStreamBuf = 0;
                mOutputStream  = 0;
                
                mRedirectStream = false;
            }
            
            // Write the header
            LogBreak();
            *mLogFile << "### " << logFile << std::endl;

            struct tm *pTime;
            time_t ctTime;
            time(&ctTime);
            pTime = localtime( &ctTime );
            *mLogFile << "### Started " << ctime( &ctTime );

            *mLogFile << "###" << std::endl;
            LogBreak();
            *mLogFile << std::endl;
        }

        /** Close the current stream, restoring redirection
        */
        void Close()
        {
            // Redirect output to the original stream:
            if ( mOutputStream )
                mOutputStream->rdbuf( mOrigStreamBuf );

            // Close the log file:
            if ( mLogFile )
                mLogFile->close();
            delete mLogFile;
            
            // Clear the section stack:
            while ( !mSections.empty() )
                mSections.pop();
        }
        
        /** Display a section break (80 #s)
        */
        void LogBreak()
        {
            *mLogFile << "################################################################################\n";
        }
        
        /** Generate the message lead-in.  This is the message-type indicator,
            the current time, and and indentations.
        */
        std::string lead_in( ErrorCode errCode = ErrorCode::Remark ) const
        {
            std::stringstream leadInStr = "";
            switch ( errCode )
            {
            case ErrorCode::Remark:
                leadInStr << "[*] ";
                break;

            case ErrorCode::Warning:
                leadInStr << "[-] ";
                break;
                
            case ErrorCode::Error:
                leadInStr << "[!] ";
                break;

            case ErrorCode::SectionStart:
                leadInStr << "[B] ";
                break;

            case ErrorCode::SectionEnd:
                leadInStr << "[E] ";
                break;
            }

            // Get the current date and time
            struct tm *pTime;
            time_t ctTime;
            time(&ctTime);
            pTime = localtime( &ctTime );

            leadInStr << std::setw( 2 ) << std::setfill( '0' ) << pTime->tm_hour
                      << ":" << std::setw( 2 ) << std::setfill( '0' ) << pTime->tm_min
                      << ":" << std::setw( 2 ) << std::setfill( '0' ) << pTime->tm_sec;

            // Add the indent
            std::string indentStr = "";
            for ( int indent = 0; indent < mSections.size(); indent++ )
                indentStr += "    ";

            return leadInStr.str() + ": " + indentStr;
        }

        /** Write a message to the logfile, including the current date and time.
            @param  msg     Message to be written to the logfile
            @param  errCode Type of message
        */
        void LogMessage( const std::string& msg, ErrorCode errCode = ErrorCode::Remark, bool appendNewline = true )
        {
            // Log the message only if the error code + mLogLevel is >= LoggingLevel::Threshold
            if ( mLogLevel == LoggingLevel::Disabled || mLogLevel + errCode < LoggingLevel::Threshold )
                return;

            std::string leadInStr = lead_in( errCode );
            std::string termChar = appendNewline ? "\n" : "";
            switch ( errCode )
            {
            case ErrorCode::Remark:
                *mLogFile << leadInStr << msg << termChar;
                break;

            case ErrorCode::Warning:
                *mLogFile << leadInStr << msg << termChar;
                break;

            case ErrorCode::Error:
                *mLogFile << leadInStr << msg << termChar;
                break;

            case ErrorCode::SectionStart:
                *mLogFile << leadInStr << "Start Section: " << msg << termChar;
                *mLogFile << leadInStr << "{" << termChar;                
                break;

            case ErrorCode::SectionEnd:
                *mLogFile << leadInStr << "}" << termChar;
                *mLogFile << leadInStr << "End Section: " << msg << termChar;
                break;
            }

            // Flush the stream to make sure the log was written
            mLogFile->flush();
        }
        
        /** Override the '<<' operator for writing simple messages. */
        template< typename T >
        inline std::ostream& operator<<( const T& val )
        {
            // Make sure the logging level is such that remarks can be logged.
            // Otherwise, write the message to cout.
            // Log the message only if the error code + mLogLevel is >= LoggingLevel::Threshold
            if ( mLogLevel == LoggingLevel::Disabled || mLogLevel + ErrorCode::Remark < LoggingLevel::Threshold )
                return std::cout << lead_in() << val;
                
            std::ostream::sentry opfx(*mLogFile);
            if ( !opfx )
                return *mLogFile;

            // This should really only be called once per chain (i.e.
            // lf << "some" << "value") as it gets converted to an ostream,
            // which handles the rest of the chain.  Write the lead_in
            // before the value.
            *mLogFile << lead_in() << val;

            // Flush the stream to make sure the log was written
            mLogFile->flush();

            return *mLogFile;
        }
    };

    /** @class LogFileSection
        @remark    
        Section group in a log file.  This is essentially a helper class for
        creating a new section which will "pop" itself when it goes out of scope.
        
        @remark
        This works well for automatically closing the section when going out of
        scope.  However, it should be noted that if there is a system crash, the
        stack will get released, but the heap may be orphaned.  This means that
        the LogFileSection's destructor will be called while the log file is
        open, thereby closing the section.  Just be aware of this if using the
        class.
    */
    class LogFileSection
    {
    private:
        LogFile*    mLogFile;               /**< Pointer to the log file containing this section */
        
    public:
        /** Constructor */
        LogFileSection( LogFile* lf, std::string sectName ) : mLogFile( lf )
        {
            if ( lf )
                lf->PushSection( sectName );
        }
        
        /** Destructor */
        ~LogFileSection()
        {
            if ( mLogFile )
                mLogFile->PopSection();
        }
        
    }; // class LogFileSection
    
}   // namespace cmd


#endif  // LOGFILE_H_
