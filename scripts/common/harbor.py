import XEngine

def on_harbor_connected(server_type, server_id):
    #连接centerd成功
    if server_type == SERVER_TYPE_CENTERD:
        
def on_harbor_accepted(server_type, server_id):
    #连接centerd成功
    my_server_type = XEngine.get_server_type()
    if my_server_type == SERVER_TYPE_CENTERD:
        #接受服务器连接成功
        pass
