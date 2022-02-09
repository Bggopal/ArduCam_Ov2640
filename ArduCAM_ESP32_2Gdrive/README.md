# UPLOADING CAPTURED IMAGES TO GDRIVE

### Requisites - 
1. base64 conversion
2. google app scripts

This requires a base64(string) conversion of image to upload to cloud, in cloud again these needs to be decoded to get the image data.
1. Using the base64 header(included) images are encoded,
2. Using the json script the images are decode.

## Creating a google app scripts

The link opens APPS Scripts in Gdrive - https://script.google.com/ , Create a new project
in code.gs, need to include the following codes(dopost method)
```
function doPost(e) {
  var myFoldername = e.parameter.myFoldername;  
  var myFile = e.parameter.myFile; 
  var myFilename = Utilities.formatDate(new Date(), "GMT+8", "yyyyMMddHHmmss")+"_"+e.parameter.myFilename;  
  
  var contentType = myFile.substring(myFile.indexOf(":")+1, myFile.indexOf(";"));
  var data = myFile.substring(myFile.indexOf(",")+1);
  data = Utilities.base64Decode(data);
  var blob = Utilities.newBlob(data, contentType, myFilename);
  
  var folder, folders = DriveApp.getFoldersByName(myFoldername);
  if (folders.hasNext()) {
    folder = folders.next();
  } else {
    folder = DriveApp.createFolder(myFoldername);
  }
  var file = folder.createFile(blob);    
  file.setDescription("Uploaded by " + myFilename);
  
  var imageID = file.getUrl().substring(file.getUrl().indexOf("/d/")+3,file.getUrl().indexOf("view")-1);
  var imageUrl = "https://drive.google.com/uc?authuser=0&id="+imageID; 
    
  return  ContentService.createTextOutput(myFoldername+"/"+myFilename+"\n"+imageUrl);
}
```
![image](https://user-images.githubusercontent.com/59637425/153227910-b4f5cfa5-f6ef-49c7-a30d-9d7a19022e63.png)

Using the deploy option, deploy the project, will lead to the deployment ID (API) need to replace the API in the script

```
String myScript = "/macros/s/________ID_______/exec";
```

![image](https://user-images.githubusercontent.com/59637425/153228803-9df7a611-9204-4d34-a752-2071b58af8db.png)

**This will decode the image and save it in the drive folder.** 

## Uploaded images in drive

![image](https://user-images.githubusercontent.com/59637425/153220611-ca6dd71c-86c7-4d11-ac63-c11f945f0df9.png)

## Script to upload data using wifi(can be found in commits)

![image](https://user-images.githubusercontent.com/59637425/153224128-96e721ab-bef0-4c0c-86da-93ed10754050.png)

## Sample to upload data using Neoway

![image](https://user-images.githubusercontent.com/59637425/153223918-f95871b7-7dec-41b8-bb7b-c71c17ced2c9.png)

