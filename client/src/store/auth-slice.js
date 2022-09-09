import { createSlice } from "@reduxjs/toolkit";

const initialState = {
    isLogged: false,
    uname: ''
};

export const authSlice = createSlice({
    name: 'auth',
    initialState,
    reducers: {
        login: (state, action) => {
            state.isLogged = true;
            state.uname = action.payload;
        }
    }
});

export const { login } = authSlice.actions;
export default authSlice.reducer;