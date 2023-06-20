const path = require('path');
const CopyPlugin = require('copy-webpack-plugin');
module.exports = {
   mode: "production",
   entry: {
      inject: path.resolve(__dirname, "src", "ts", "inject", "inject.ts"),
      background: path.resolve(__dirname, "src", "ts", "background", "background.ts"),
      popup: path.resolve(__dirname, "src", "ts", "popup", "popup.ts"),
   },
   output: {
      path: path.join(__dirname, "dist", "js"),
      filename: "[name].bundle.js",
   },
   resolve: {
      extensions: [".ts", ".js"],
   },
   module: {
      rules: [
         {
            test: /\.tsx?$/,
            loader: "ts-loader",
            exclude: /node_modules/,
         },
      ],
   },
   plugins: [
    new CopyPlugin({
      patterns: [{
         from: path.resolve(__dirname, "src"), 
         to: path.resolve(__dirname, "dist"),
         globOptions: {
            dot: true,
            gitignore: true,
            ignore: ["**/ts/**"],
          }
         }]
     }),
   ],
};
