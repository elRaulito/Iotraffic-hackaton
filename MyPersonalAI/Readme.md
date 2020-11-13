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

# How to make a request?

You can select any image you want to test, encode it in base64 with tools like [this](https://base64.guru/converter/encode/image)

```
curl --location --request POST 'http://localhost:3000/image' \
--header 'Content-Type: application/json' \
--data-raw '{
    "image": "image encoded in base64 in plain text"
}'

```

The response will look something like this:




```
[
    {
        "bbox": [
            12.013011932373047,
            28.14077541232109,
            323.8699836730957,
            224.03340235352516
        ],
        "class": "bicycle",
        "score": 0.931533932685852
    },
    {
        "bbox": [
            73.09247589111328,
            1.23288094997406,
            176.43450164794922,
            142.04309940338135
        ],
        "class": "person",
        "score": 0.557381808757782
    }
]

```

The bbox is the square containing the object, the class is the object detected and the score is the probability of being correct
