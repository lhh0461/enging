def rpc_server_alliance_req(player_entity, alliance_id, player_id):
    print("rpc call success!!", alliance_id) 
    remote_call("alliance_mgr", "get_alliance_info_req", player_entity.id, alliance_id)

def rpc_host_alliance_resp(player_entity, alliance_id, alliance_name):
    print("rpc call success!!", alliance_id) 
    
