import { fetchEventSource } from "@microsoft/fetch-event-source";
import { store } from "../store/store";
import { pushMessage } from "../store/chat-area-slice";
import { reloadChat } from "../store/current-chat-slice";

export const subscribeToChatRoom = (chatroom, dispatch) => {
  const url = new URL('http://127.0.0.1:8080/sseevents');
  const params = { subscribe: chatroom };
  url.search = new URLSearchParams(params).toString();
  
  fetchEventSource(url, {
    onmessage(msg) {
      const data = JSON.parse(msg.data);

      const uname = store.getState().auth.uname;
      const new_msg = {
        id: data.chatroom,
        message: {
          content: data.message,
          type: uname === data.sender ? 'sent' : 'receive'
        }
      };      
      dispatch(pushMessage(new_msg));
      
      const currentChatRoom = store.getState().currentChat.id;
      if (currentChatRoom === data.chatroom) {
        const chats = store.getState().chatAreas.chats;
        const chat = chats.find(chat => {
          return chat.id === currentChatRoom;
        });
        dispatch(reloadChat(chat));
      }      
    }
  });
}
