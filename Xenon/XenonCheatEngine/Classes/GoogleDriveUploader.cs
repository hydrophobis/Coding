using System;
using System.IO;
using System.Threading;
using System.IO.Compression;
using Google.Apis.Auth.OAuth2;
using Google.Apis.Auth.OAuth2.Flows;
using Google.Apis.Auth.OAuth2.Responses;
using Google.Apis.Drive.v3;
using Google.Apis.Services;
using Google.Apis.Util.Store;
using ScriptKidAntiCheat.Utils;
using System.Collections.Generic;

// Deprecated for security reasons

namespace ScriptKidAntiCheat
{
    public class GoogleDriveUploader
    {
        // If modifying these scopes, remember to generate new token
        static string[] Scopes = { DriveService.Scope.DriveFile };
        // ClientId & ClientSecret needs to be created at google developer console
        static readonly ClientSecrets secrets = new ClientSecrets()
        {

        };
        // Refresh token is generate by generateNewToken(); see line 41
        public string refreshToken = "";

        public GoogleDriveUploader()
        {

        }

        public void UploadFile(FileInfo ReplayFile)
        {

        }

        public UserCredential generateNewToken()
        {
          
        }

        private UserCredential AuthorizeWithRefreshToken(string token)
        {

        }

        private string ZipDirectory(FileInfo ReplayFile)
        {
 
        }
    }
}
