import gloal_config
import XEngine

import centerd.start_up.on_centerd_start_up as OnCenterdStartUp
import gamed.start_up.on_gamed_start_up as OnGamedStartUp
import gated.start_up.on_gated_start_up as OnGatedStartUp

START_UP_DISPATCH = {
    globalconfig.SERVER_TYPE_CENTERD:OnCenterdStartUp,
    globalconfig.SERVER_TYPE_GAMED:OnGamedStartUp,
    globalconfig.SERVER_TYPE_GATED:OnGatedStartUp,
}

def on_start_up:
    print("on script start up")

def script_start_up():
    server_type = XEngine.get_server_type()
    if server_type in START_UP_DISPATCH:
        if START_UP_DISPATCH[server_type]() != True:
            print("start up dispatch fail")
    else:
        on_start_up()

