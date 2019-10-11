import XEngine

#刚开服时监听有足够多的游戏服连上来才开始创建服务
def on_centerd_start_up():
    XEngine.create_entity("server_mgr")

def on_centerd_start_up():
    
