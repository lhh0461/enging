#全局服务管理模块
import XEngine

#当entity创建时调用
def __on_create__(entity):
    #start timer
    XEngine.register_global_service("service_mgr", entity)

#当entity注册成功后调用
def __on_registered__(entity, result):
    if result == True:
        init_global_entity()

def init_global_entity():
    XEngine.create_entity_anywhere("alliance_mgr")
    XEngine.create_entity_anywhere("friend_mgr")
