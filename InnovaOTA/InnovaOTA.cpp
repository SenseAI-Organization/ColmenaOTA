
/** ****************************************************************************
 ** ************ INCLUDES ******************************************************
 ** ****************************************************************************/
#include "InnovaOTA.h"

extern const char *firmwareVer;

/** ****************************************************************************
 ** ************ DEFINES *******************************************************
 ** ****************************************************************************/
// URLs to query new firmware version
#define URL_FW_VER "https://raw.githubusercontent.com/SenseAI-Organization/ColmenaOTA/main/bin_version.txt"
#define URL_FW_BIN "https://raw.githubusercontent.com/SenseAI-Organization/ColmenaOTA/main/firmware.bin"


/** ****************************************************************************
 ** ************ VARIABLES *****************************************************
 ** ****************************************************************************/
const char *rootCACertificate = "-----BEGIN CERTIFICATE-----\n"
"MIIHOTCCBiGgAwIBAgIQBj1JF0BNOeUTyz/uzRsuGzANBgkqhkiG9w0BAQsFADBZ\n"
"MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMTMwMQYDVQQDEypE\n"
"aWdpQ2VydCBHbG9iYWwgRzIgVExTIFJTQSBTSEEyNTYgMjAyMCBDQTEwHhcNMjQw\n"
"MzE1MDAwMDAwWhcNMjUwMzE0MjM1OTU5WjBnMQswCQYDVQQGEwJVUzETMBEGA1UE\n"
"CBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNU2FuIEZyYW5jaXNjbzEVMBMGA1UEChMM\n"
"R2l0SHViLCBJbmMuMRQwEgYDVQQDDAsqLmdpdGh1Yi5pbzCCASIwDQYJKoZIhvcN\n"
"AQEBBQADggEPADCCAQoCggEBAK0rFKU6TEGvuLCY3ZOuXlG+3jerD6EP1gc1qe35\n"
"g68FqyGuVPOUddYNZiymjYMZxywoNp3qxlbFFBTf9etsayavT+uW+2UMjqCotAdK\n"
"KicBEspuExoACFuNgTi7sSUT7A55+k4/+5O+VtpaxQ5dmQk7HxcqvMYx5owBU+fB\n"
"wYDD+hXeg3YvxLZNeIlN8OlqWL8w9HbG+3ccegVEjOJQbkrcrW7IQMq2Uk92XjxI\n"
"PmMVIvaefqcC1poGYvS4VvEh3x64vJK1hEM4YLMKBaE/hqFtcMozi+H/8JqTCfzP\n"
"Qhnu21HIop9rSucxxnZbe9AeHz2LERpUTf3rjgOMg9PB1RUCAwEAAaOCA+0wggPp\n"
"MB8GA1UdIwQYMBaAFHSFgMBmx9833s+9KTeqAx2+7c0XMB0GA1UdDgQWBBTob1fr\n"
"hlGY65+lvlPa25SsKC777TB7BgNVHREEdDByggsqLmdpdGh1Yi5pb4IJZ2l0aHVi\n"
"LmlvghVnaXRodWJ1c2VyY29udGVudC5jb22CDnd3dy5naXRodWIuY29tggwqLmdp\n"
"dGh1Yi5jb22CFyouZ2l0aHVidXNlcmNvbnRlbnQuY29tggpnaXRodWIuY29tMD4G\n"
"A1UdIAQ3MDUwMwYGZ4EMAQICMCkwJwYIKwYBBQUHAgEWG2h0dHA6Ly93d3cuZGln\n"
"aWNlcnQuY29tL0NQUzAOBgNVHQ8BAf8EBAMCBaAwHQYDVR0lBBYwFAYIKwYBBQUH\n"
"AwEGCCsGAQUFBwMCMIGfBgNVHR8EgZcwgZQwSKBGoESGQmh0dHA6Ly9jcmwzLmRp\n"
"Z2ljZXJ0LmNvbS9EaWdpQ2VydEdsb2JhbEcyVExTUlNBU0hBMjU2MjAyMENBMS0x\n"
"LmNybDBIoEagRIZCaHR0cDovL2NybDQuZGlnaWNlcnQuY29tL0RpZ2lDZXJ0R2xv\n"
"YmFsRzJUTFNSU0FTSEEyNTYyMDIwQ0ExLTEuY3JsMIGHBggrBgEFBQcBAQR7MHkw\n"
"JAYIKwYBBQUHMAGGGGh0dHA6Ly9vY3NwLmRpZ2ljZXJ0LmNvbTBRBggrBgEFBQcw\n"
"AoZFaHR0cDovL2NhY2VydHMuZGlnaWNlcnQuY29tL0RpZ2lDZXJ0R2xvYmFsRzJU\n"
"TFNSU0FTSEEyNTYyMDIwQ0ExLTEuY3J0MAwGA1UdEwEB/wQCMAAwggF/BgorBgEE\n"
"AdZ5AgQCBIIBbwSCAWsBaQB2AE51oydcmhDDOFts1N8/Uusd8OCOG41pwLH6ZLFi\n"
"mjnfAAABjkN89oAAAAQDAEcwRQIgU/M527Wcx0KQ3II7kCuG5WMuOHRSxKkf1xAj\n"
"JuSkyPACIQCVX0uurcIA2Ug7ipNN2S1ZygukWqJCh7hjIH0XsrXh8QB2AH1ZHhLh\n"
"eCp7HGFnfF79+NCHXBSgTpWeuQMv2Q6MLnm4AAABjkN89oEAAAQDAEcwRQIgCxpL\n"
"BDak+TWKarrCHlZn4DlqwEfAN3lvlgSo21HQuU8CIQDicrb72c0lA2suMWPWT92P\n"
"FLaRvFrFn9HVzI6Vh50YZgB3AObSMWNAd4zBEEEG13G5zsHSQPaWhIb7uocyHf0e\n"
"N45QAAABjkN89pQAAAQDAEgwRgIhAPJQX4QArFCjM0sKKzsWLmqmmU8lMhKEYR2T\n"
"ges1AQyQAiEA2Y3VhP5RG+dapcbwYgVbrTlgWzO7KE/lg1x11CVcz3QwDQYJKoZI\n"
"hvcNAQELBQADggEBAHKlvzObJBxxgyLaUNCEFf37mNFsUtXmaWvkmcfIt9V+TZ7Q\n"
"mtvjx5bsd5lqAflp/eqk4+JYpnYcKWrZfM/vMdxPQTeh/VQWewY/hYn6X/V1s2JI\n"
"MtjqEkW4aotVdWjHVvsx4rAjz5vtub/wVYgtrU8jusH3TVpT9/0AoFhKE5m2IS7M\n"
"Ig7wKR+DDxoNj4fFFluxteVNgbtwuJcb23NkBQqfHXCvQWqxXZZA4Nwl/WoGPoGG\n"
"dW5qVOc3BlhtITW53ASyhvKC7HArhj7LwQH8C/dRgn1agIHP9vVJ1NaZnPXhK98T\n"
"ohv++OO0E/F/bVGNWVnLBQ4v5PjQzRQUTGvM2mU=\n"
"-----END CERTIFICATE-----\n";
/* ***************************************************************************
 * **** FUNCTIONS ************************************************************
 * ***************************************************************************/
// firmwareVersionCheck()
// @param none: void
// @return status: int
// Función auxiliar para comparar versiones
bool isNewerVersion(const char* serverVersion, const char* deviceVersion) {
    int serverNums[3] = {0};
    int deviceNums[3] = {0};
    
    // Parsear versión del servidor
    sscanf(serverVersion, "%d.%d.%d", &serverNums[0], &serverNums[1], &serverNums[2]);
    
    // Parsear versión del dispositivo
    sscanf(deviceVersion, "%d.%d.%d", &deviceNums[0], &deviceNums[1], &deviceNums[2]);
    
    // Comparar versiones
    for(int i = 0; i < 3; i++) {
        if(serverNums[i] > deviceNums[i]) return true;
        if(serverNums[i] < deviceNums[i]) return false;
    }
    
    return false; // Si son iguales, no es una versión más nueva
}

// Modificar la función firmwareVersionCheck para usar la nueva lógica
int firmwareVersionCheck() {
    if (WiFi.status() != WL_CONNECTED) return -1;
    
    HTTPClient http;
    http.begin("https://raw.githubusercontent.com/SenseAI-Organization/ColmenaOTA/main/bin_version.txt");
    
    int httpCode = http.GET();
    if(httpCode != HTTP_CODE_OK) {
        http.end();
        return -1;
    }
    
    String newFWVersion = http.getString();
    http.end();
    
    // Limpiar cualquier caracter no deseado
    newFWVersion.trim();
    
    Serial.print("Server version: ");
    Serial.println(newFWVersion);
    Serial.print("Device version: ");
    Serial.println(firmwareVer);
    
    // Usar la nueva función de comparación
    if(isNewerVersion(newFWVersion.c_str(), firmwareVer)) {
        return 1;
    }
    
    return 0;
}

// firmwareUpdate()
// @param none: void
// @return none: void
bool firmwareUpdate()
{
    WiFiClientSecure wificlient;
    wificlient.setInsecure(); //

    t_httpUpdate_return ret = httpUpdate.update(wificlient, URL_FW_BIN);

    switch (ret)
    {
    case HTTP_UPDATE_FAILED:
        Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
        return false;
        
    case HTTP_UPDATE_NO_UPDATES:
        Serial.println("HTTP_UPDATE_NO_UPDATES");
        return false;
        
    case HTTP_UPDATE_OK:
        Serial.println("HTTP_UPDATE_OK");
        return true;
    }
    
    return false; // Por si acaso llegamos aquí
}

/****** END OF FILE ****/

/*
if (firmwareVersionCheck())
{
    firmwareUpdate();
}
else
{
    // Serial.println("No new firmware detected");
}
*/