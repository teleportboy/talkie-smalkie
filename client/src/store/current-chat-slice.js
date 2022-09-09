import { createSlice } from "@reduxjs/toolkit";

const initialState = {
  id: 'SELECT_CHAT_ROOM',
  messages: [

  ]
};

export const currentChatSlice = createSlice({
  name: 'currentChat',
  initialState,
  reducers: {
    reloadChat: (state, action) => {
      state.id = action.payload.id;
      state.messages = action.payload.messages;
    }
  }
});

export const { reloadChat } = currentChatSlice.actions;
export default currentChatSlice.reducer;