# Edit following two lines to set component requirements (see docs)
set(COMPONENT_REQUIRES )
set(COMPONENT_PRIV_REQUIRES )

set(COMPONENT_SRCS "main.c" connect.c fetch.c server.c camera.c tasks.c)
set(COMPONENT_ADD_INCLUDEDIRS "")


set(COMPONENT_EMBED_TXTFILES
    "html/index.html"
    "html/gateway.html"
    "html/wifi.html"
    "html/limit.html"
    "html/deviceid.html"
    "html/boot/js/bootstrap.min.js"
    "html/boot/css/bootstrap.min.css"
    "html/boot/jquery/jquery.min.js"

)

set(COMPONENT_EMBED_FILES
    "html/img/cover.png"
)

register_component()
