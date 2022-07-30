import React from "react";

export function Login() {
  return (
    <form className="login">
      <div className="username-input item">
        <label htmlFor="uname"><b>Username</b></label>
        <input type="text" placeholder="Enter Username" name="uname" required />
      </div>
      <div className="password-input item">
        <label htmlFor="psw"><b>Password</b></label>
        <input type="password" placeholder="Enter Password" name="psw" required />
      </div>
      <div className="login-btn item">
        <button type="submit"><b>Loooogin</b></button>
      </div>
      <div className="new-acc-btn item">
        <button type="button">Create new account</button>
      </div>
    </form>
  );
}