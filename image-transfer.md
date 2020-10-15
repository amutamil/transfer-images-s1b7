## <center>Mc3File

- Mc3File is an application that is used to generate images in *DICOM* format.

- It takes in as args __service and command name__ which can be viewed by using -l flag and __number of images__ to be generated.

- The generated file will have an extension of __*.00x*__ which should be renamed to __*.img*__ 
for Sender application.

- Mc3File folder has the mc3file.exe and the required dll's and lib files.

- ex: mc3file BASIC_FILM_BOX N_DELETE_RSP 1

<br>

## <center>  STORE_SCP & Sender

 __*stor_scp*__ is an application used to recieve generated dicom images.

 __*Sender*__ is an application used to send generated Dicom images over to the server.

<br>

## <center> EmailSender Python Utility

 __*filepolling.py*__ is used to poll the Receiver Directory for any new files that are  received.

 __*Sendmail.py*__ is used to send the mail to notify the Radiologist.
