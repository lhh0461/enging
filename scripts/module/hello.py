import google.protobuf 

def rpc_server_hello_req(action_id, flag, req_str, value):
    print("rpc call success!!",action_id, flag, req_str, value) 