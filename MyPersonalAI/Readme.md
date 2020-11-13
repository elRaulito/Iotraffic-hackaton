## MyPersonalAI a Free object detector neural network

A node application capable of detecting objects in base64 images that are sent to it.
Thanks to the work of [Sean12697](https://github.com/Sean12697/MobileNet-via-TensorFlowJS-in-NodeJS) who made a local decoder in node, I added a base64 decoder and the server port for POST request. Instead of using mobilenet model I use coco-ssd of tensorflow.
# How to install it?

download the folder MypersonalAI and inside of it run 

```
npm install 
```

Now you can run the AI with the command 

```
node index.js
```
The server is started listening on http://localhost:3000
